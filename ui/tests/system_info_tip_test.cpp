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

#include <QFrame>
#include <QLineEdit>

#include "ui/widgets/system_info_tip.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QFrame frame;

  QLineEdit line_edit(&frame);
  line_edit.setFixedWidth(650);
  installer::SystemInfoTip tooltip(&frame);
  tooltip.hide();

  QObject::connect(&line_edit, &QLineEdit::textChanged,
      [&](const QString& text) {
        tooltip.setText(text);
        tooltip.showBottom(&line_edit);
      });

  frame.resize(840, 480);
  line_edit.move(20, 20);
  frame.show();

  return app.exec();
}