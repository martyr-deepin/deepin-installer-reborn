/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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
#include <QTextCodec>

#include "base/consts.h"
#include "ui/user_form_window.h"

int main(int argc, char** argv) {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QApplication app(argc, argv);
  app.setApplicationVersion(installer::kAppVersion);

  installer::UserFormWindow window;
  window.fullscreen();

  return app.exec();
}