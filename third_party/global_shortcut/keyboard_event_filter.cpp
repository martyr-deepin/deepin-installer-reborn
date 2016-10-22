// Copyright (c) 2015 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#include "keyboard_event_filter.h"

#include <QAbstractEventDispatcher>
#include <QDebug>
#include <QX11Info>
#include <xcb/xcb.h>
#include <X11/Xlib.h>

namespace {

// Flag used to mark XGrabKey() and XUnGrabKey() error.
bool g_grab_ungrab_error = false;

static int qxt_x_errhandler(Display* display, XErrorEvent* event) {
  Q_UNUSED(display);
  switch (event->error_code) {
    // Pass through
    case BadAccess:
    case BadValue:
    case BadWindow: {
      if (event->request_code == 33 /* X_GrabKey */ ||
          event->request_code == 34 /* X_UngrabKey */) {
        g_grab_ungrab_error = true;
      }
      break;
    }

    default:
      break;
  }

  return 0;
}

}  // namespace

KeyboardEventFilter::KeyboardEventFilter() : shortcuts_() {
  QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
}

KeyboardEventFilter::~KeyboardEventFilter() {
  QAbstractEventDispatcher::instance()->removeNativeEventFilter(this);
}

// Filters native X event
bool KeyboardEventFilter::nativeEventFilter(const QByteArray& event_type,
                                            void* message, long* result) {
  Q_UNUSED(result);
  if (event_type == "xcb_generic_event_t") {
    xcb_generic_event_t* event = static_cast<xcb_generic_event_t*>(message);
    if ((event->response_type & ~0x80) == XCB_KEY_PRESS) {
      xcb_key_press_event_t* kp = (xcb_key_press_event_t*)event;

      const quint32 keycode = kp->detail;
      const quint32 keymod =
          static_cast<quint32>(kp->state & (ShiftMask | ControlMask |
                                            Mod1Mask | Mod4Mask));

      // Active shortcut if available.
      GlobalShortcut* shortcut = shortcuts_.value(qMakePair(keycode, keymod));
      if (shortcut && shortcut->isRegistered()) {
        emit shortcut->activated();
      }
    }
  }
  return false;
}

bool KeyboardEventFilter::registerShortcut(quint32 nativeKey,
                                           quint32 nativeMods) {
  Display* display = QX11Info::display();
  if (!display) {
    qWarning() << "registerShortcut() display is null";
    return false;
  }
  Window window = QX11Info::appRootWindow();
  Bool owner = True;
  int pointer = GrabModeAsync;
  int keyboard = GrabModeAsync;
  g_grab_ungrab_error = false;
  auto original_x_errhandler = XSetErrorHandler(qxt_x_errhandler);
  XGrabKey(display, nativeKey, nativeMods, window, owner, pointer, keyboard);
  XGrabKey(display, nativeKey, nativeMods | Mod2Mask, window, owner, pointer,
           keyboard); // allow numlock
  XSync(display, False);
  XSetErrorHandler(original_x_errhandler);
  return !g_grab_ungrab_error;
}

bool KeyboardEventFilter::unregisterShortcut(quint32 nativeKey,
                                             quint32 nativeMods) {
  Display* display = QX11Info::display();
  Q_ASSERT(display);
  Window window = QX11Info::appRootWindow();
  g_grab_ungrab_error = false;
  auto original_x_errhandler = XSetErrorHandler(qxt_x_errhandler);
  XUngrabKey(display, nativeKey, nativeMods, window);
  XUngrabKey(display, nativeKey, nativeMods | Mod2Mask,
             window); // allow numlock
  XSync(display, False);
  XSetErrorHandler(original_x_errhandler);
  return !g_grab_ungrab_error;
}