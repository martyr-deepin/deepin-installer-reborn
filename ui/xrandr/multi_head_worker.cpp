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

#include "ui/xrandr/multi_head_worker.h"

#include <QDebug>

#ifdef Status
#undef Status
#endif
#ifdef Bool
#undef Bool
#endif
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xrandr.h>

namespace installer {

MultiHeadWorker::MultiHeadWorker(QObject* parent)
    : QObject(parent),
      is_running_(false) {
  this->setObjectName("multi_head_worker");

  this->initConnections();
}

MultiHeadWorker::~MultiHeadWorker() {
  Q_ASSERT(!is_running_);
  if (is_running_) {
    is_running_ = false;
  }
}

void MultiHeadWorker::stop() {
  is_running_ = false;
}

void MultiHeadWorker::initConnections() {
  connect(this, &MultiHeadWorker::start,
          this, &MultiHeadWorker::doStart);
}

void MultiHeadWorker::doStart() {
  if (is_running_) {
    return;
  }

  is_running_ = true;

  Display* dpy = NULL;
  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    qCritical() << "Failed to open X display";
    return;
  }

  int screen = DefaultScreen(dpy);
  if (screen >= ScreenCount(dpy)) {
    qCritical() << "Invalid screen number:" << screen
                << ", display has" << ScreenCount(dpy);
    return;
  }

  Window root = RootWindow(dpy, screen);
  XRRSelectInput(dpy, root, RROutputChangeNotifyMask |
                            RRScreenChangeNotifyMask);

  int event_base, error_base;
  int major, minor;
  if (!XRRQueryExtension(dpy, &event_base, &error_base) ||
      !XRRQueryVersion(dpy, &major, &minor)) {
    qCritical() << "RandR extension missing!";
    return;
  }

  XEvent event;

  while (is_running_) {
    XNextEvent(dpy, &event);
    XRRUpdateConfiguration(&event);

    switch (event.type - event_base) {
      case RRScreenChangeNotify: {
        emit this->monitorsChanged();
        break;
      }
      default: {
        // pass
        break;
      }
    }
  }

  XCloseDisplay(dpy);
}

}  // namespace installer