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

// Absolute path to installer config file.
const char kInstallerConfigFile[] = "/etc/deepin-installer.conf";

// File name of auto partition script.
const char kAutoPartFile[] = "auto-part.sh";

// Absolute path to oem dir in system ISO.
// Note that iso image is mounted at "/lib/live/mount/medium/".
const char kOemDir[] = "/lib/live/mount/medium/oem";

bool AppendToConfigFile(const QString& line) {
  QFile file(kInstallerConfigFile);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    qWarning() << "Failed to append to config file:" << line;
    return false;
  }
  file.write(line.toUtf8());
  file.close();
  return true;
}

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

QString GetAutoPartFile() {
  const QString oem_file = QDir(kOemDir).absoluteFilePath(kAutoPartFile);
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  const QString builtin_file =
      QDir(BUILTIN_HOOKS_DIR).absoluteFilePath(kAutoPartFile);
  if (QFile::exists(builtin_file)) {
    return builtin_file;
  }

  qCritical() << "GetAutoPartFile() not partition script found!";
  return QString();
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

QString GetOverlayFilesystemDir() {
  return QDir(kOemDir).absoluteFilePath(QStringLiteral("overlay"));
}

QString GetVendorLogo() {
  const QString oem_file = QDir(kOemDir).absoluteFilePath("logo.png");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  // Returns builtin logo.
  return QStringLiteral(":/images/logo.png");
}

QString GetWindowBackground() {
  const QString oem_file = QDir(kOemDir).absoluteFilePath("background.jpg");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  const QString in_system = GetSettingsString(
      kSystemInfoDdeDefaultWallpaperName);
  if (QFile::exists(in_system)) {
    return in_system;
  }

  return QStringLiteral(":/resources/fallback-wallpaper.jpg");
}

bool DeleteConfigFile() {
  QFile file(kInstallerConfigFile);
  if (file.exists()) {
    if (!file.remove()) {
      qCritical() << "Failed to delete installer config file!";
      return false;
    }
  }
  return true;
}

void WriteLocale(const QString& locale) {
  const QString line = QString("DI_LOCALE=%1\n").arg(locale);
  AppendToConfigFile(line);
}

void WriteUsername(const QString& username) {
  const QString line = QString("DI_USERNAME=%1\n").arg(username);
  AppendToConfigFile(line);
}

void WriteHostname(const QString& hostname) {
  const QString line = QString("DI_HOSTNAME=%1\n").arg(hostname);
  AppendToConfigFile(line);
}

void WritePassword(const QString& password) {
  // TODO(xushaohua): base64 encode
  const QString line = QString("DI_PASSWORD=%1\n").arg(password);
  AppendToConfigFile(line);
}

void WriteAvatar(const QString& avatar) {
  const QString line = QString("DI_AVATAR=%1\n").arg(avatar);
  AppendToConfigFile(line);
}

void WriteTimezone(const QString& timezone) {
  const QString line = QString("DI_TIMEZONE=%1\n").arg(timezone);
  AppendToConfigFile(line);
}

void WriteKeyboard(const QString& layout, const QString& variant) {
  const QString line = QString("DI_LAYOUT=%1\n").arg(layout);
  AppendToConfigFile(line);
  const QString line2 = QString("DI_LAYOUT_VARIANT=%1\n").arg(variant);
  AppendToConfigFile(line2);
}

void WritePartitionInfo(const QString& root, const QString& mount_points) {
  const QString line = QString("DI_ROOT_PARTITION=%1\n").arg(root);
  AppendToConfigFile(line);
  const QString line2 = QString("DI_MOUNTPOINTS=%1\n").arg(mount_points);
  AppendToConfigFile(line2);
  // TODO(xushaohua): Add DI_ROOT_DISK
}

}  // namespace service
