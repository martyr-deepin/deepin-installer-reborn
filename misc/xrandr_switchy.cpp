// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QDebug>

#include "ui/utils/xrandr_wrapper.h"

// XRandR Switchy

int main(void) {
  QString out;
  if (!installer::RunXRandR(out)) {
    qCritical() << "Run xrandr failed";
    return 1;
  }
  if (out.isEmpty()) {
    qCritical() << "xrandr returns an empty string!";
    return 1;
  }

  installer::XRandR xrandr;
  if (!installer::ParseXRandR(out, xrandr)) {
    qCritical() << "Parse XRandR failed!";
    return 1;
  }
  qDebug() << "xrandr:" << xrandr;

  if (!installer::SwitchMode(xrandr)) {
    qCritical() << "Switch xrandr mode failed!";
    return 1;
  }

  return 0;
}