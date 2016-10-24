// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/utils/multi_head_worker.h"

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
  this->setObjectName(QStringLiteral("multi_head_worker"));

  this->initConnections();
}

MultiHeadWorker::~MultiHeadWorker() {
  Q_ASSERT(!is_running_);
  if (is_running_) {
    is_running_ = false;
  }
}

void MultiHeadWorker::initConnections() {
  connect(this, &MultiHeadWorker::start,
          this, &MultiHeadWorker::doStart);
  connect(this, &MultiHeadWorker::stop,
          this, &MultiHeadWorker::doStop);
}

void MultiHeadWorker::doStart() {
  qDebug() << "MultiHeadWorker:: do start()";
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
  XRRSelectInput(dpy, root, RROutputChangeNotifyMask | RRScreenChangeNotifyMask);

  int event_base, error_base;
  int major, minor;
  if (!XRRQueryExtension(dpy, &event_base, &error_base) ||
      !XRRQueryVersion(dpy, &major, &minor)) {
    qCritical() << "RandR extension missing!";
    return;
  }
  qDebug() << "RandR version:" << major << minor;

  XEvent event;

  while (is_running_) {
    XNextEvent(dpy, &event);

    qDebug() << "Event received, type=" << event.type;
    XRRUpdateConfiguration(&event);
    if (event.type == ConfigureNotify) {
      qDebug() << "Received configureNotify event!";
    }

    switch (event.type - event_base) {
      case RRScreenChangeNotify: {
        qDebug() << "RRScreenChangeNotify!";

        XRRScreenChangeNotifyEvent* sce = NULL;
        sce = (XRRScreenChangeNotifyEvent*) &event;
        qDebug() << "window:" << int(sce->window)
                 << "width:" << sce->width
                 << "height:" << sce->height;
        emit this->screenCountChanged();
        break;
      }
      default: {
        // pass
        break;
      }
    }
  }

}

void MultiHeadWorker::doStop() {
  is_running_ = false;
}

}  // namespace installer