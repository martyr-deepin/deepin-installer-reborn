// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QCoreApplication>
#include <QDebug>

#include "service/locale_manager.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);

  installer::LocaleManager manager;
  QObject::connect(&manager, &installer::LocaleManager::localeUpdated,
    [&](const QString& locale) {
    qDebug() << "locale updated:" << locale;
  });
  QObject::connect(&manager, &installer::LocaleManager::timezoneUpdated,
    [&](const QString& timezone) {
      qDebug() << "timezone updated:" << timezone;
    });
  manager.update();

  return app.exec();
}