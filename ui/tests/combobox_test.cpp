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
  box.addItems({"Debian", "Ubuntu", "RedHat", "OpenSuse", "Fedora",
                "ArchLinux"});
  box.move(100, 100);
  frame.resize(480, 360);
  frame.setAttribute(Qt::WA_TranslucentBackground, true);
  frame.show();

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
//      "QMenu::item:first {"
//      "  border-top-left-radius: 8px;"
//      "  border-top-right-radius: 8px;"
//      "}"
      "QMenu::item {"
      "  background: transparent;"
//      "  background: rgba(0, 0, 0, 0.3);"
      "  color: #fff;"
      "}"
      "QMenu::item:hover {"
      "  background: rgba(255, 255, 255, 0.3);"
      "}"
  );
  frame.setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(&frame, &QWidget::customContextMenuRequested,
      [&]() {
        menu.popup(QCursor::pos());
      });

  return app.exec();
}