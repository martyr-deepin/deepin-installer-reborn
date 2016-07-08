// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <QFile>

namespace service {

SettingsManager::SettingsManager() : QObject() {
  this->setObjectName(QStringLiteral("settings_manager"));
}

QString SettingsManager::getWindowBackground() {
  // TODO(xushaohua): Find image in root of iso.

  const QString in_system(QStringLiteral("/usr/share/backgrounds/default.jpg"));
  if (QFile::exists(in_system)) {
    return in_system;
  }

  return QStringLiteral(":/misc/fallback-wallpaper.jpg");
}

}  // namespace service