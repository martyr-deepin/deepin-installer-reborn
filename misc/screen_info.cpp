// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>

static void PrintScreen(QDesktopWidget* desktop_widget) {
  const int screen_count = desktop_widget->screenCount();
  for (int i = 0; i < screen_count; i++) {
    const QRect geom = desktop_widget->screenGeometry(i);
    qDebug() << "Screen: " << i << geom;
  }
  const QList<QScreen*> screen_list = qApp->screens();
  for (QScreen* screen : screen_list) {
    qDebug() << "QScreen:" << screen->name()
             << "geometry:" << screen->geometry()
             << "available geometry:" << screen->availableGeometry();
  }
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QDesktopWidget* desktop_widget = QApplication::desktop();
  QObject::connect(desktop_widget, &QDesktopWidget::primaryScreenChanged,
      [&]() {
        qDebug() << "primary screen changed:"
                 << desktop_widget->primaryScreen();
        PrintScreen(desktop_widget);
      });
  QObject::connect(desktop_widget, &QDesktopWidget::screenCountChanged,
      [&]() {
        qDebug() << "screen count changed:" << desktop_widget->screenCount();
        PrintScreen(desktop_widget);
      });
  PrintScreen(desktop_widget);

  return app.exec();
}