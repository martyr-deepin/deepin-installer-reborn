// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QCoreApplication>
#include <QDebug>

#include "service/timezone_manager.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);

  installer::TimezoneManager manager;
  QObject::connect(&manager, &installer::TimezoneManager::timezoneUpdated,
    [&](const QString& timezone) {
      qDebug() << "timezone updated:" << timezone;
    });
  manager.update(false, true);

  return app.exec();
}