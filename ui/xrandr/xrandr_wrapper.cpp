/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui/xrandr/xrandr_wrapper.h"

#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include "base/command.h"

namespace installer {

namespace {

const char kRandRCmd[] = "xrandr";

struct _crtc_t {
  RRCrtc id;
  RRMode mode;
  int x;
  int y;
  int width;
  int height;
  Rotation rotation;
  int noutput;
  RROutput* outputs;
  bool changed;
};
typedef struct _crtc_t crtc_t;

// Initialized |crtc| object with default values.
void InitCrtc(crtc_t* crtc) {
  Q_ASSERT(crtc != NULL);

  memset(crtc, 0, sizeof(crtc_t));
  crtc->rotation = RR_Rotate_0;
  crtc->outputs = NULL;
  crtc->changed = false;
}

// Release memories used by crtc->outputs.
void DestroyCrtc(crtc_t* crtc) {
  if (crtc && crtc->noutput > 0) {
    free(crtc->outputs);
    crtc->outputs = NULL;
    crtc->noutput = 0;
  }
}

// Destroy crtc_t objects created in GetCrtcs() method.
void DestroyCrtcs(crtc_t** crtcs, int& num_crtcs) {
  if (*crtcs == NULL) {
    return;
  }

  for (int i = 0; i < num_crtcs; ++i) {
    crtc_t* crtc = (*crtcs) + i;
    DestroyCrtc(crtc);
  }
  free(*crtcs);
  *crtcs = NULL;
  num_crtcs = 0;
}

// Get |crtcs| from |resources|, |num_crtcs| holds number of crtc_t objects.
// Remember to call DestroyCrtcs() to release memories.
// Returns false on error.
bool GetCrtcs(Display* dpy, XRRScreenResources* resources,
              crtc_t** crtcs, int& num_crtcs) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  num_crtcs = resources->ncrtc;
  *crtcs = static_cast<crtc_t*>(calloc((size_t)num_crtcs, sizeof(crtc_t)));
  if (*crtcs == NULL) {
    return false;
  }

  // Initialize crtc list.
  for (int i = 0; i < resources->ncrtc; ++i) {
    crtc_t* crtc = (*crtcs) + i;
    InitCrtc(crtc);
    XRRCrtcInfo* rr_crtc_info = XRRGetCrtcInfo(dpy, resources,
                                               resources->crtcs[i]);
    if (rr_crtc_info == NULL) {
      // Release resources.
      DestroyCrtcs(crtcs, num_crtcs);

      fprintf(stderr, "XRRGetCrtcInfo() returns NULL\n");
      return false;
    }
    crtc->id = resources->crtcs[i];
    crtc->rotation = rr_crtc_info->rotation;
    crtc->x = rr_crtc_info->x;
    crtc->y = rr_crtc_info->y;
    crtc->width = rr_crtc_info->width;
    crtc->height = rr_crtc_info->height;
    crtc->mode = rr_crtc_info->mode;

    crtc->noutput = rr_crtc_info->noutput;
    if (crtc->noutput > 0) {
      crtc->outputs = static_cast<RROutput*>(calloc((size_t)crtc->noutput,
                                                        sizeof(RROutput)));
      if (crtc->outputs == NULL) {
        // Release resources.
        DestroyCrtcs(crtcs, num_crtcs);

        fprintf(stderr, "GetCrtcs() failed to alloc mem for outputs\n");
        return false;
      }

      // Copy output list.
      memcpy(crtc->outputs, rr_crtc_info->outputs,
             sizeof(RROutput) * crtc->noutput);
    }
    XRRFreeCrtcInfo(rr_crtc_info);
  }

  return true;
}

// Apply changed defined in crtc list.
bool ApplyCrtcs(Display* dpy, XRRScreenResources* resources,
                crtc_t* crtcs, int num_crtcs) {
#ifndef N_DEBUG
  fprintf(stdout, "ApplyCrtcs() crts: %d\n", num_crtcs);
#endif
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  Q_ASSERT(crtcs != NULL);

  bool ok = true;
  for (int i = 0; i < num_crtcs; ++i) {
    crtc_t* crtc = crtcs + i;
    if (crtc == NULL || crtc->noutput <= 0 || !crtc->changed) {
#ifndef N_DEBUG
      fprintf(stdout, "ignores crtcs[%d]\n", i);
      if (crtc == NULL) {
        fprintf(stdout, "crtcs[%d] is null\n", i);
      } else if (crtc->noutput <= 0) {
        fprintf(stdout, "crtcs[%d] output list is empty\n", i);
      } else {
        fprintf(stdout, "crtcs[%d] not changed\n", i);
      }
#endif
      continue;
    }
    XGrabServer(dpy);

#ifndef N_DEBUG
    fprintf(stdout, "XRRSetCrtcConfig() %dx%d+%d+%d, %d\n",
            crtc->x, crtc->y, crtc->width, crtc->height, crtc->noutput);
#endif
    Status status = XRRSetCrtcConfig(dpy, resources, crtc->id,
                                     CurrentTime, crtc->x, crtc->y,
                                     crtc->mode, crtc->rotation,
                                     crtc->outputs, crtc->noutput);
    if (status != RRSetConfigSuccess) {
      ok = false;
      fprintf(stderr, "Failed to update crtc\n");
    }
    XUngrabServer(dpy);
  }

  return ok;
}

// Add |output| to |crtcs| crtc list.
bool AddOutputToCrtcs(crtc_t* crtcs, int num_crtc, RROutput output) {
  for (int i = 0; i < num_crtc; ++i) {
    crtc_t* crtc = crtcs + i;
    if (crtc->noutput == 0) {
      // Release old outputs if found.
      Q_ASSERT(crtc->outputs == NULL);
      if (crtc->outputs != NULL) {
        free(crtc->outputs);
      }
      // Allocate memory for |output|.
      crtc->outputs = static_cast<RROutput*>(calloc(1, sizeof(RROutput)));
      if (crtc->outputs == NULL) {
        return false;
      }
      // Save output value.
      *crtc->outputs = output;
      crtc->noutput = 1;
      crtc->changed = true;
      return true;
    }
  }
  return false;
}

// A helper function to get x11 display and randr resources.
// Returns false if any of these objects are not initialized successfully.
// Remember to release |resources| and close |dpy| after using them.
bool GetRRResources(Display** dpy, Window& root_window,
                    XRRScreenResources** resources) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  // Get default display.
  *dpy = XOpenDisplay(NULL);
  if (*dpy == NULL) {
    fprintf(stderr, "Failed to open display\n");
    return false;
  }

  // Get default screen.
  const int screen = DefaultScreen(*dpy);
  if (screen < 0 || screen >= ScreenCount(*dpy)) {
    XCloseDisplay(*dpy);

    fprintf(stderr, "Invalid screen: %d (display has %d)\n",
            screen, ScreenCount(*dpy));
    return false;
  }

  root_window = RootWindow(*dpy, screen);
  int event_base, error_base;
  int major, minor;

  // Query RandR extension.
  if (!XRRQueryExtension(*dpy, &event_base, &error_base) ||
      !XRRQueryVersion(*dpy, &major, &minor)) {
    XCloseDisplay(*dpy);

    fprintf(stderr, "RandR extension missing\n");
    return false;
  }
#ifndef N_DEBUG
  fprintf(stdout, "RandR version %d.%d\n", major, minor);
#endif

  *resources = XRRGetScreenResources(*dpy, root_window);
  if (*resources == NULL) {
    XCloseDisplay(*dpy);

    fprintf(stderr, "Failed to get screen resources.\n");
    return false;
  }
  return true;
}

// Check number of outputs is less or equal to 1.
bool IsOnlyOneOutput(Display* dpy, XRRScreenResources* resources) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  int connected_outputs = 0;
  for (int i = 0; i < resources->noutput; ++i) {
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources,
                                                  resources->outputs[i]);
    if (output_info && (output_info->connection == RR_Connected)) {
      connected_outputs ++;
    }
    XRRFreeOutputInfo(output_info);
  }
  return connected_outputs <= 1;
}

bool IsMirrorMode(Display* dpy, XRRScreenResources* resources) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  // Get connected outputs.
  // Check whether geometry modes of connected outputs equals each other.

  unsigned int width = 0;
  unsigned int height = 0;
  int x = 0;
  int y = 0;
  bool mode_inited = false;

  for (int i = 0; i < resources->noutput; ++i) {
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources,
                                                  resources->outputs[i]);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }

    XRRCrtcInfo* crtc_info = XRRGetCrtcInfo(dpy, resources, output_info->crtc);
    XRRFreeOutputInfo(output_info);
    if (crtc_info == NULL) {
      fprintf(stderr, "crtc_info is null\n");
      continue;
    }

    if (!mode_inited) {
      mode_inited = true;
      width = crtc_info->width;
      height = crtc_info->height;
      x = crtc_info->x;
      y = crtc_info->y;
    } else {
      if (width != crtc_info->width ||
          height != crtc_info->height ||
          x != crtc_info->x ||
          y != crtc_info->y) {
        XRRFreeCrtcInfo(crtc_info);
        return false;
      }
    }

    XRRFreeCrtcInfo(crtc_info);
  }

  return true;
}

// Get mode info with |mode_id|.
// Do not free returned XRRModeInfo object.
XRRModeInfo* GetModeInfoById(Display* dpy, XRRScreenResources* resources,
                             RRMode mode_id) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  for (int i = 0; i < resources->nmode; ++i) {
    XRRModeInfo* mode_info = resources->modes + i;
    if (mode_info->id == mode_id) {
      return mode_info;
    }
  }
  return NULL;
}

// Get preferred mode for |output_info|.
XRRModeInfo* GetPreferModeInfo(Display* dpy, XRRScreenResources* resources,
                               XRROutputInfo* output_info) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  Q_ASSERT(output_info != NULL);

  XRRModeInfo* best = NULL;
  int bestDist = 0;
  int screen = 0;

  for (int m = 0; m < output_info->nmode; m++) {
    XRRModeInfo* mode_info = GetModeInfoById(dpy, resources,
                                             output_info->modes[m]);
    int dist = 0;

    if (m < output_info->npreferred) {
      dist = 0;
    } else if (output_info->mm_height) {
      dist = int(1000 * DisplayHeight(dpy, screen) / DisplayHeightMM(dpy, screen) -
              1000 * mode_info->height / output_info->mm_height);
    } else {
      dist = DisplayHeight(dpy, screen) - mode_info->height;
    }

    if (dist < 0) {
      dist = -dist;
    }
    if (!best || dist < bestDist) {
      best = mode_info;
      bestDist = dist;
    }
  }

  return best;
}

// Walk through connected outputs to get best same mode supported by all the
// connected outputs.
bool GetBestSameRRMode(Display* dpy, XRRScreenResources* resources,
                       unsigned int* best_width, unsigned int* best_height) {
  for (int mode_idx = 0; mode_idx < resources->nmode; ++mode_idx) {
    Q_ASSERT(dpy != NULL);
    Q_ASSERT(resources != NULL);

    // Walk through global mode list.
    XRRModeInfo* global_mode_info = resources->modes + mode_idx;
    bool global_found_same_mode = true;
    for (int output_idx = 0; output_idx < resources->noutput; ++output_idx) {
      bool local_found_same_mode = false;

      RROutput output = resources->outputs[output_idx];
      XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
      if (output_info == NULL || output_info->connection != RR_Connected) {
        XRRFreeOutputInfo(output_info);
        continue;
      }

      for (int k = 0; k < output_info->nmode; ++k) {
        RRMode local_mode = output_info->modes[k];
        XRRModeInfo* local_mode_info = GetModeInfoById(dpy, resources,
                                                       local_mode);

        if (local_mode_info == NULL) {
          continue;
        }

        if (local_mode_info->width == global_mode_info->width &&
            local_mode_info->height == global_mode_info->height) {
          local_found_same_mode = true;
          break;
        }
      }

      // global_mode_info does not match any mode in current output.
      if (!local_found_same_mode) {
        global_found_same_mode = false;
        XRRFreeOutputInfo(output_info);
        break;
      }

      XRRFreeOutputInfo(output_info);
    }

    // Found best matched mode for all connected outputs.
    if (global_found_same_mode) {
      *best_width = global_mode_info->width;
      *best_height = global_mode_info->height;
      return true;
    }
  }

  return false;
}

// Walk through connected outputs to get best same mode supported by all the
// connected outputs.
//RRMode GetBestSameRRMode(Display* dpy, XRRScreenResources* resources) {
//  Q_ASSERT(dpy != NULL);
//  Q_ASSERT(resources != NULL);
//
//  for (int i = 0; i < resources->noutput; ++i) {
//    XRROutputInfo* output_info_i = XRRGetOutputInfo(dpy, resources,
//                                                    resources->outputs[i]);
//    if (output_info_i == NULL || output_info_i->connection != RR_Connected) {
//      // Ignores disconnected outputs.
//      XRRFreeOutputInfo(output_info_i);
//      continue;
//    }
//
//    for (int j = i + 1; j < resources->noutput; ++j) {
//      XRROutputInfo* output_info_j = XRRGetOutputInfo(dpy, resources,
//                                                      resources->outputs[j]);
//      if (output_info_j == NULL || output_info_j->connection != RR_Connected) {
//        XRRFreeOutputInfo(output_info_j);
//        // Ignores disconnected outputs.
//        continue;
//      }
//
//      for (int mode_index = 0; mode_index < resources->nmode; ++mode_index) {
//        XRRModeInfo* mode_info = resources->modes + mode_index;
//        if (mode_info == NULL) {
//          continue;
//        }
//
//        for (int mode_index_i = 0; mode_index_i < output_info_i->nmode;
//             ++mode_index_i) {
//          RRMode mode_i = output_info_i->modes[mode_index_i];
//          if (mode_info->id != mode_i) {
//            continue;
//          }
//
//          for (int mode_index_j = 0; mode_index_j < output_info_j->nmode;
//               ++mode_index_j) {
//            RRMode mode_j = output_info_j->modes[mode_index_j];
//            if (mode_info->id != mode_j) {
//              continue;
//            }
//#ifndef N_DEBUG
//            fprintf(stdout, "Same mode: %ld %s, %dx%d\n",
//                    mode_info->id, mode_info->name,
//                    mode_info->width, mode_info->height);
//#endif
//
//            // Release all output_info.
//            XRRFreeOutputInfo(output_info_i);
//            XRRFreeOutputInfo(output_info_j);
//
//            return mode_info->id;
//          }
//        }
//      }
//    }
//
//    XRRFreeOutputInfo(output_info_i);
//  }
//
//  return None;
//}

// Get index of primary output in connected output list.
void GetPrimaryOutputIndex(Display* dpy, XRRScreenResources* resources,
                           Window root_window, int* connected_outputs,
                           int* primary_index) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  *connected_outputs = 0;
  *primary_index = -1;
  // primary_output might be None.
  RROutput primary_output = XRRGetOutputPrimary(dpy, root_window);
  for (int i = 0; i < resources->noutput; ++i) {
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources,
                                                  resources->outputs[i]);
    if (output_info != NULL && output_info->connection == RR_Connected) {
      // Check primary output.
      if (primary_output == resources->outputs[i]) {
        *primary_index = *connected_outputs;
      }

      (*connected_outputs) ++;
    }
    XRRFreeOutputInfo(output_info);
  }
}

// Update screen size based on current crtc.
bool UpdateScreenSize(Display* dpy, Window root_window,
                      XRRScreenResources* resources,
                      crtc_t* crtcs, int num_crtcs) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  Q_ASSERT(crtcs != NULL);

  int fb_width = 0;
  int fb_height = 0;

  for (int i = 0; i < num_crtcs; ++i) {
    crtc_t* crtc = crtcs + i;
    if (crtc->noutput == 0) {
      continue;
    }
    // Extended horizontally.
    fb_width = qMax(fb_width, crtc->x + crtc->width);
    fb_height = qMax(fb_height, crtc->y + crtc->height);
  }

  // Get default screen.
  int screen = DefaultScreen(dpy);
  if (screen < 0 || screen >= ScreenCount(dpy)) {
    fprintf(stderr, "Invalid screen: %d (display has %d)\n",
            screen, ScreenCount(dpy));
    return false;
  }

  // Compute physical screen size.
  if (fb_width == DisplayWidth(dpy, screen) &&
      fb_height == DisplayHeight(dpy, screen)) {
#ifndef N_DEBUG
    fprintf(stdout, "No need to update screen size\n");
#endif
    return true;
  }
//  const double delta = 25.4;
//  double dpi = (delta * DisplayHeight (dpy, screen)) /
//               DisplayHeightMM(dpy, screen);
//  const int fb_width_mm = int((delta * fb_width) / dpi);
//  const int fb_height_mm = int((delta * fb_height) / dpi);
  // From xfce4-display.
  // The 'physical size' of an X screen is meaningless if that screen
  // can consist of many monitors. So just pick a size that make the
  // dpi 96.
  // Firefox and Evince apparently believe what X tells them.
  const int fb_width_mm = int(fb_width / 96.0 * 25.4 + 0.5);
  const int fb_height_mm = int(fb_height / 96.0 * 25.4 + 0.5);

#ifndef N_DEBUG
  fprintf(stdout, "Set screen size: %d, %d, %d, %d\n",
          fb_width, fb_height, fb_width_mm, fb_height_mm);
#endif

  XRRSetScreenSize(dpy, root_window, fb_width, fb_height,
                   fb_width_mm, fb_height_mm);

  return true;
}

// Switch to mirror mode.
// Checkout number of connected outputs before calling this method.
bool ToMirrorMode(Display* dpy, XRRScreenResources* resources,
                  Window root_window, crtc_t* crtcs, int num_crtcs) {
#ifndef N_DEBUG
  fprintf(stdout, "ToMirrorMode()\n");
#endif
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  Q_UNUSED(root_window);
  Q_ASSERT(crtcs != NULL);
  Q_ASSERT(num_crtcs > 0);
  // Get mode with same size of connected outputs.
  // Update crtcs.
  // Apply crtcs.

  for (int i = 0; i < resources->noutput; ++i) {
    const RROutput output = resources->outputs[i];
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }

    bool found_output = false;
    for (int j = 0; j < num_crtcs; ++j) {
      crtc_t* crtc = crtcs + j;
      for (int k = 0; k < crtc->noutput; ++k) {
        if (output == crtc->outputs[k]) {
          found_output = true;
          break;
        }
      }
    }

    // Add Output into crtc list if not found.
    if (!found_output) {
#ifndef N_DEBUG
      fprintf(stdout, "Add output to crtc list: %s\n", output_info->name);
#endif
      if (!AddOutputToCrtcs(crtcs, num_crtcs, output)) {
        fprintf(stderr, "Failed to add output to crtc list: %s\n",
                output_info->name);
        XRRFreeOutputInfo(output_info);
        return false;
      }
    }

    XRRFreeOutputInfo(output_info);
  }

  unsigned int best_width = 0;
  unsigned int best_height = 0;
  const bool found_best_mode = GetBestSameRRMode(dpy, resources,
                                                 &best_width, &best_height);
  fprintf(stdout, "best mode: %d, w: %d, h:%d\n",
          found_best_mode, best_width, best_height);

  QStringList args;

  for (int i = 0; i < resources->noutput; ++i) {
    RROutput output = resources->outputs[i];
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }

    XRRModeInfo* best_mode_info = NULL;
    for (int k = 0; k < output_info->nmode; ++k) {
      const RRMode mode = output_info->modes[k];
      XRRModeInfo* mode_info = GetModeInfoById(dpy, resources, mode);
      if (mode_info == NULL) {
        continue;
      }

      // Returns the first available mode if no best matched mode is found.
      if (!found_best_mode) {
        best_mode_info = mode_info;
        break;
      }

      fprintf(stdout, "i: %d, mode info: %dx%d\n",
              k, mode_info->width, mode_info->height);

      if ((mode_info->width == best_width) &&
          (mode_info->height == best_height)) {
        best_mode_info = mode_info;
        break;
      }
    }

    args << "--output" << output_info->name << "--pos" << "0x0"
         << "--mode" << best_mode_info->name;

    XRRFreeOutputInfo(output_info);
  }
  qDebug() << "args:" << args;

  return SpawnCmd(kRandRCmd, args);

//  RROutput primary_output = None;
//
//  // Update crtcs.
//  for (int i = 0; i < num_crtcs; ++i) {
//    crtc_t* crtc = crtcs + i;
//    if (crtc->noutput <= 0) {
//      fprintf(stdout, "ToMirroMode() ignored crtc: %ld\n", crtc->id);
//      continue;
//    }
//
//    // Select the first connected output as primary one.
//    for (int j = 0; j < crtc->noutput; ++j) {
//      fprintf(stdout, "output: %d\n", j);
//      RROutput output = crtc->outputs[j];
//      XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
//      if (output_info != NULL && output_info->connection == RR_Connected) {
//#ifndef N_DEBUG
//        fprintf(stdout, "Select %s as primary output\n", output_info->name);
//#endif
//        if (primary_output == None) {
//          primary_output = output;
//        }
//      }
//
//      RRMode best_mode = None;
//      XRRModeInfo* best_mode_info = NULL;
//      for (int k = 0; k < output_info->nmode; ++k) {
//        const RRMode mode = output_info->modes[k];
//        XRRModeInfo* mode_info = GetModeInfoById(dpy, resources, mode);
//        if (mode_info == NULL) {
//          continue;
//        }
//
//        // Returns the first available mode if no best matched mode is found.
//        if (!found_best_mode) {
//          best_mode = mode;
//          best_mode_info = mode_info;
//          break;
//        }
//
//        fprintf(stdout, "i: %d, mode info: %dx%d\n",
//                k, mode_info->width, mode_info->height);
//
//        if ((mode_info->width == best_width) &&
//            (mode_info->height == best_height)) {
//          best_mode = mode;
//          best_mode_info = mode_info;
//          break;
//        }
//      }
//
//      crtc->mode = best_mode;
//      crtc->changed = true;
//      crtc->x = 0;
//      crtc->y = 0;
//      crtc->width = best_mode_info->width;
//      crtc->height = best_mode_info->height;
//      fprintf(stdout, "update crtc info, mode: %ld, w: %d, h: %d\n",
//              best_mode, crtc->width, crtc->height);
//
//      XRRFreeOutputInfo(output_info);
//    }
//  }
//
//  // Set primary output.
//  XRRSetOutputPrimary(dpy, root_window, primary_output);
//
//  // Update crtc first.
//  if (!ApplyCrtcs(dpy, resources, crtcs, num_crtcs)) {
//    fprintf(stderr, "Apply crtcs failed\n");
//    return false;
//  }
//
//  // Update screen size.
//  return UpdateScreenSize(dpy, root_window, resources, crtcs, num_crtcs);
}

// Switch to next mode.
bool ToNextMode(Display* dpy, XRRScreenResources* resources,
                Window root_window, crtc_t* crtcs, int num_crtcs) {
  // Get primary output index.
  // Check current mode.
  // Update crtc based on mode.
  // Apply crtc.
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);
  Q_ASSERT(crtcs != NULL);
  Q_ASSERT(num_crtcs > 0);

  int connected_outputs = 0;
  int primary_index = -1;
  GetPrimaryOutputIndex(dpy, resources, root_window,
                        &connected_outputs, &primary_index);

  // If no connected output, do nothing.
  if (connected_outputs == 0) {
    fprintf(stderr, "No connected output\n");
    return true;
  }

  // If primary output is the last connected output, and there are multiple
  // connected outputs, go to mirror mode.
  if ((connected_outputs > 1) &&
      (primary_index > -1) &&
      (primary_index + 1 == connected_outputs) &&
      (!IsMirrorMode(dpy, resources))) {
#ifndef N_DEBUG
    fprintf(stdout, "Switch to mirror mode, %d, %d\n",
            primary_index, connected_outputs);
#endif
    return ToMirrorMode(dpy, resources, root_window, crtcs, num_crtcs);
  }

  // Index of output in connected output list to be set as primary one.
  int next_primary_index = -1;
  if (connected_outputs == 1) {
    // If only one output is found, use its preferred mode.
    next_primary_index = 0;
  } else if (primary_index == -1 || IsMirrorMode(dpy, resources)) {
    // If no primary output is set, or in mirror mode, set the first connected
    // output as primary output.
    next_primary_index = 0;
  } else {
    // Switch primary output to next one in output list.
    next_primary_index = primary_index + 1;
    Q_ASSERT(next_primary_index < connected_outputs);
  }

  // Order outputs horizontally.
  int x = 0;
  const int y = 0;
  int connected_outputs2 = 0;
  for (int output_index = 0; output_index < resources->noutput;
       ++output_index) {
    const RROutput output = resources->outputs[output_index];
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }

    // Set primary.
    if (connected_outputs2 == next_primary_index) {
#ifndef N_DEBUG
      fprintf(stdout, "Set primary: %s\n", output_info->name);
#endif
      XRRSetOutputPrimary(dpy, root_window, output);
    }
    connected_outputs2 ++;

    XRRModeInfo* prefer = GetPreferModeInfo(dpy, resources, output_info);
    Q_ASSERT(prefer != NULL);

    // Use preferred mode.
    for (int crtc_index = 0; crtc_index < num_crtcs; ++crtc_index) {
      crtc_t* crtc = crtcs + crtc_index;
      if (crtc->id == output_info->crtc) {
        crtc->changed = true;
        crtc->x = x;
        crtc->y = y;
        if (crtc->mode != prefer->id) {
          crtc->mode = prefer->id;
          crtc->width = prefer->width;
          crtc->height = prefer->height;
        }
        x = crtc->width + x;
      }
    }

    // Release output_info.
    XRRFreeOutputInfo(output_info);
  }

  Q_ASSERT(connected_outputs == connected_outputs2);

  // Update crtc first.
  if (!ApplyCrtcs(dpy, resources, crtcs, num_crtcs)) {
    return false;
  }

  // Update screen size.
  return UpdateScreenSize(dpy, root_window, resources, crtcs, num_crtcs);
}

// Set all connected output as prefer mode.
bool ToPreferMode(Display* dpy, XRRScreenResources* resources) {
  Q_ASSERT(dpy != NULL);
  Q_ASSERT(resources != NULL);

  QStringList args;
  for (int i = 0; i < resources->noutput; ++i) {
    RROutput output = resources->outputs[i];
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }

    args << "--output" << output_info->name << "--preferred";

    XRRFreeOutputInfo(output_info);
  }
  qDebug() << "args:" << args;

  return SpawnCmd(kRandRCmd, args);
}

}  // namespace

QDebug& operator<<(QDebug& debug, const ConnectedOutput& output) {
  debug << "Output: {"
        << "x:" << output.x
        << "y:" << output.y
        << "width:" << output.width
        << "height:" << output.height
        << "primary:" << output.primary
        << "}";
  return debug;
}

bool GetConnectedOutputs(ConnectedOutputs& outputs) {
  Display* dpy = NULL;
  Window root_window;
  XRRScreenResources* resources = NULL;
  if (!GetRRResources(&dpy, root_window, &resources)) {
    return false;
  }

  outputs.clear();
  // Get primary output.
  const RROutput primary = XRRGetOutputPrimary(dpy, root_window);
  for (int i = 0; i < resources->noutput; ++i) {
    const RROutput output_id = resources->outputs[i];
    XRROutputInfo* output_info = XRRGetOutputInfo(dpy, resources, output_id);
    if (output_info == NULL || output_info->connection != RR_Connected) {
      XRRFreeOutputInfo(output_info);
      continue;
    }
    ConnectedOutput output_obj = {};
    if (output_id == primary) {
      output_obj.primary = true;
    }

    // Update connected output properties.
    const RRCrtc crtc = output_info->crtc;
    XRRCrtcInfo* crtc_info = XRRGetCrtcInfo(dpy, resources, crtc);
    if (crtc_info == NULL) {
      fprintf(stderr, "crtc_info of %s is null\n", output_info->name);
      output_obj.x = 0;
      output_obj.y = 0;
      output_obj.width = 0;
      output_obj.height = 0;
    } else {
      output_obj.x = crtc_info->x;
      output_obj.y = crtc_info->y;
      output_obj.width = crtc_info->width;
      output_obj.height = crtc_info->height;
    }
    // Release output_info.
    XRRFreeOutputInfo(output_info);
    outputs.append(output_obj);
    XRRFreeCrtcInfo(crtc_info);
  }

  XRRFreeScreenResources(resources);
  XCloseDisplay(dpy);
  return true;
}

bool SwitchMode() {
  Display* dpy = NULL;
  Window root_window;
  XRRScreenResources* resources = NULL;
  if (!GetRRResources(&dpy, root_window, &resources)) {
    return false;
  }

  crtc_t* crtcs = NULL;
  int num_crtcs = 0;
  if (!GetCrtcs(dpy, resources, &crtcs, num_crtcs)) {
    // Release resources.
    XRRFreeScreenResources(resources);
    XCloseDisplay(dpy);

    fprintf(stderr, "Failed to get crtc_t objects\n");
    return false;
  }

  if (!ToNextMode(dpy, resources, root_window, crtcs, num_crtcs)) {
    fprintf(stderr, "ToNextMode() failed\n");
  }

  DestroyCrtcs(&crtcs, num_crtcs);
  XRRFreeScreenResources(resources);
  XCloseDisplay(dpy);

  return true;
}

bool SwitchToMirrorMode() {
#ifndef N_DEBUG
  fprintf(stdout, "SwitchToMirrorMode()\n");
#endif
  Display* dpy = NULL;
  Window root_window;
  XRRScreenResources* resources = NULL;
  if (!GetRRResources(&dpy, root_window, &resources)) {
    fprintf(stderr, "Failed to get rr resources\n");
    return false;
  }

  // Check number of outputs.
  if (IsOnlyOneOutput(dpy, resources)) {
    XRRFreeScreenResources(resources);
    XCloseDisplay(dpy);

    fprintf(stdout, "only one output, do nothing.\n");
    return true;
  }

  crtc_t* crtcs = NULL;
  int num_crtcs = 0;
  if (!GetCrtcs(dpy, resources, &crtcs, num_crtcs)) {
    // Release resources.
    XRRFreeScreenResources(resources);
    XCloseDisplay(dpy);

    fprintf(stderr, "Failed to get crtc_t objects\n");
    return false;
  }

  if (!ToMirrorMode(dpy, resources, root_window, crtcs, num_crtcs)) {
    fprintf(stderr, "ToMirrorMode() failed\n");
  }

  DestroyCrtcs(&crtcs, num_crtcs);
  XRRFreeScreenResources(resources);
  XCloseDisplay(dpy);

  return true;
}

bool SwitchToPreferMode() {
  Display* dpy = NULL;
  Window root_window;
  XRRScreenResources* resources = NULL;
  if (!GetRRResources(&dpy, root_window, &resources)) {
    return false;
  }
  Q_UNUSED(root_window);

  if (!ToPreferMode(dpy, resources)) {
    fprintf(stderr, "ToNextMode() failed\n");
  }

  XRRFreeScreenResources(resources);
  XCloseDisplay(dpy);

  return true;
}

}  // namespace installer
