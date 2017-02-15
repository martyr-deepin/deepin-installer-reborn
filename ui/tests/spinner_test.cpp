// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "ui/widgets/spinner_label.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  installer::SpinnerLabel label;
  label.setAttribute(Qt::WA_TranslucentBackground, true);
  label.setWindowFlags(Qt::FramelessWindowHint);
  label.show();
  label.start();

  return app.exec();
}