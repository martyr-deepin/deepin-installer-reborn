// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

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

  const QString oem_file =
      oem_dir_.absoluteFilePath(QStringLiteral("settings.ini"));
  oem_settings_ = new QSettings(oem_file , QSettings::IniFormat, this);
  default_settings_ = new QSettings(
      QStringLiteral(":/resources/default-settings.ini"),
      QSettings::IniFormat, this);
}

bool SettingsManager::getSettingsBool(const QString& key) {
  const QVariant value = this->getSettingsValue(key);
  if (value.isValid()) {
    return value.toBool();
  }
  return false;
}

QString SettingsManager::getSettingsString(const QString& key) {
  const QVariant value = this->getSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }
  return QStringLiteral("");
}

QVariant SettingsManager::getSettingsValue(const QString& key) {
  if (oem_settings_->contains(key)) {
    return oem_settings_->value(key);
  }

  if (default_settings_->contains(key)) {
    return default_settings_->value(key);
  }

  qWarning() << "[SettingsManager]::getSettingsValue() Invalid key:" << key;

  return QVariant();
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
