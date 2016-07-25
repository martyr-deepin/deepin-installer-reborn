// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <time.h>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QSettings>

#include "service/settings_name.h"

namespace service {

namespace {

// Absolute path to oem dir in system ISO.
// Note that iso image is mounted at "/lib/live/mount/medium/".
const char kOemDir[] = "/lib/live/mount/medium/oem";

QStringList ListImageFiles(const QString& dir_name) {
  QStringList result;
  QDir dir(dir_name);
  if (!dir.exists()) {
    return result;
  }

  const QStringList name_filters = { "*.png", "*.jpg" };
  const QFileInfoList info_list =
      dir.entryInfoList(name_filters, QDir::NoDotAndDotDot | QDir::Files);
  for (const QFileInfo& info : info_list) {
    if (info.size() > 0) {
      result.append(info.absoluteFilePath());
    }
  }

  return result;
}

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

QStringList GetAvatars() {
  // First, check oem/ dir.
  const QString oem_dir(QDir(kOemDir).absoluteFilePath("avatar"));
  QStringList avatars = ListImageFiles(oem_dir);
  if (!avatars.isEmpty()) {
    return avatars;
  }

  // Then, check dde-account-faces dir.
  return ListImageFiles(GetSettingsString(kSystemInfoDdeAvatarDirName));
}

QString GetDefaultAvatar() {
  // Return user specified avatar.
  if (GetSettingsBool(kSystemInfoUseDefaultAvatorName)) {
    return GetSettingsString(kSystemInfoDefaultAvatorName);
  }

  // Pick a random avatar.
  const int num = static_cast<int>(time(NULL));
  const QStringList avatars = GetAvatars();
  if (avatars.isEmpty()) {
    return "";
  }
  const int index = num % avatars.length();
  return avatars.at(index);
}

QString GetOemHooksDir() {
  return QDir(kOemDir).absoluteFilePath(QStringLiteral("hooks"));
}

QString GetVendorLogo() {
  if (GetSettingsBool(kSystemInfoUseVendorLogoName)) {
    return QDir(kOemDir).absoluteFilePath("logo.png");
  }
  return QStringLiteral(":/images/logo.png");
}

QString GetWindowBackground() {
  if (GetSettingsBool(kSystemInfoUseOemWallpaperName)) {
    return QDir(kOemDir).absoluteFilePath("background.jpg");
  }

  const QString in_system =
      GetSettingsString(kSystemInfoDdeDefaultWallpaperName);
  if (QFile::exists(in_system)) {
    return in_system;
  }

  return QStringLiteral(":/resources/fallback-wallpaper.jpg");
}

}  // namespace service
