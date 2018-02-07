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
#include <QMenu>

#include "ui/widgets/table_combo_box.h"
#include "ui/utils/widget_util.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QFrame frame;
  installer::TableComboBox box(&frame);
  box.addItems({"Debian", "Ubuntu", "RedHat", "OpenSuse", "Fedora",
                "ArchLinux"});
  box.move(100, 100);
  frame.resize(480, 360);
  frame.setAttribute(Qt::WA_TranslucentBackground, true);
  frame.show();

  installer::WidgetTreeWalk(&box);

  QMenu menu;
  menu.addAction("Help");
  menu.addAction("About");
  menu.addAction("Quit");
  menu.setAttribute(Qt::WA_TranslucentBackground, true);
  menu.setStyleSheet(
      "QMenu {"
      "  background: rgba(0, 0, 0, 0.6);"
      "  border-radius: 8px;"
      "  border: 1px solid rgba(0, 0, 0, 0.5);"
      "}"
      "QMenu::item:first {"
      "  border-top-left-radius: 8px;"
      "  border-top-right-radius: 8px;"
      "  background: red;"
      "}"
      "QMenu::item {"
      "  background: transparent;"
      "  color: #fff;"
      "  text-align: center;"
      " qproperty-alignment: AlignCenter;"
      "}"
      "QMenu::item:selected {"
      "  background: red;"
      "  color: rgba(255, 255, 255, 0.9);"
      "}"
  );
  frame.setContextMenuPolicy(Qt::CustomContextMenu);

  QObject::connect(&frame, &QWidget::customContextMenuRequested,
      [&]() {
        menu.popup(QCursor::pos());
      });
  installer::WidgetTreeWalk(&menu);

//  QGraphicsDropShadowEffect effect;
//  effect.setXOffset(3);
//  effect.setYOffset(3);
////  effect.setColor(QColor::fromRgbF(0, 0, 0, 0.5));
//  menu.setGraphicsEffect(&effect);

  return app.exec();
}