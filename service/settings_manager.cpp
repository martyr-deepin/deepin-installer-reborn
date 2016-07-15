// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

namespace service {

namespace {

// Absolute path to oem dir in system ISO.
// Note that iso image is mounted at "/lib/live/mount/medium/".
const char kOemDir[] = "/lib/live/mount/medium/oem";

}  // namespace

bool GetSettingsBool(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toBool();
  }

  qCritical() << "GetSettingsBool() failed with key:" << key;
  return false;
}

QString GetSettingsString(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }

  qCritical() << "GetSettingsString() failed with key:" << key;
  return QStringLiteral("");
}

QVariant GetSettingsValue(const QString& key) {
  const QString oem_file =
      QDir(kOemDir).absoluteFilePath(QStringLiteral("settings.ini"));

  // TODO(xushaohua): Set as static variables to speed up.
  QSettings oem_settings(oem_file , QSettings::IniFormat);
  QSettings default_settings(
      QStringLiteral(":/resources/default-settings.ini"),
      QSettings::IniFormat);

  if (oem_settings.contains(key)) {
    return oem_settings.value(key);
  }

  if (default_settings.contains(key)) {
    return default_settings.value(key);
  }

  qWarning() << "[SettingsManager]::getSettingsValue() Invalid key:" << key;
  return QVariant();
}

QString GetWindowBackground() {
  const QString in_oem =
      QDir(kOemDir).absoluteFilePath(QStringLiteral("background.jpg"));
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
