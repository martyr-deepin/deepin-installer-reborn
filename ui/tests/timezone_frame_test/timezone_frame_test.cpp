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