// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QFrame>
#include <QMenu>

#include "ui/widgets/table_combo_box.h"
#include "ui/utils/widget_util.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QFrame frame;
  installer::TableComboBox box(&frame);
  box.addItems({"Debian", "Ubuntu", "RedHat", "OpenSusue", "Fedora",
                "ArchLinux"});
  box.move(100, 100);
  frame.resize(480, 360);
  frame.setAttribute(Qt::WA_TranslucentBackground, true);
  frame.setAttribute(Qt::WA_NoBackground, true);
  frame.setStyleSheet("background: transparent;");
  frame.show();

  QMenu menu;
  menu.addAction("Help");
  menu.addAction("About");
  menu.addAction("Quit");
  menu.popup(QPoint(100, 100));
  menu.setStyleSheet("background: transparent;");
  menu.setAttribute(Qt::WA_TranslucentBackground, true);
  installer::SetQMenuTransparent(&menu);

  return app.exec();
}