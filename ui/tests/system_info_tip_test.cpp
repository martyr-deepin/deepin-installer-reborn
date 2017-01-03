// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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