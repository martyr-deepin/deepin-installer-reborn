// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <QFile>

namespace service {

namespace {

// Absolute path to oem dir in system ISO.
// Note that iso image is mounted at "/lib/live/mount/medium/".
const char kOemDir[] = "/lib/live/mount/medium/oem";

}  // namespace

SettingsManager::SettingsManager()
    : QObject(),
      oem_dir_(kOemDir) {
  this->setObjectName(QStringLiteral("settings_manager"));
}

QString SettingsManager::getWindowBackground() {
  const QString in_oem =
      oem_dir_.absoluteFilePath(QStringLiteral("background.jpg"));
  if (QFile::exists(in_oem)) {
    return in_oem;
  }

  const QString in_system(QStringLiteral("/usr/share/backgrounds/default.jpg"));
  if (QFile::exists(in_system)) {
    return in_system;
  }

  return QStringLiteral(":/resources/fallback-wallpaper.jpg");
}

}  // namespace service
