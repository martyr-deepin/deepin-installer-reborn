// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "ui/frames/timezone_frame.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  installer::TimezoneFrame frame;
  frame.setAttribute(Qt::WA_TranslucentBackground, true);
//  frame.setWindowFlags(Qt::FramelessWindowHint);
  frame.show();
  QObject::connect(&frame, &installer::TimezoneFrame::finished,
                   &app, &QApplication::quit);

  return app.exec();
}