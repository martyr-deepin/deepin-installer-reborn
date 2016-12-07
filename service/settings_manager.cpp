// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/settings_manager.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QHash>
#include <QSettings>

#include "service/settings_name.h"

namespace installer {

namespace {

// Absolute path to oem dir.
QString g_oem_dir;

// Absolute path to installer config file.
const char kInstallerConfigFile[] = "/etc/deepin-installer.conf";

// Absolute path to global oem config file saved in system.
const char kInstallerOemConfigFile[] = "/etc/deepin-installer-oem.conf";

// Absolute path to default installer settings
const char kDefaultSettingsFile[] = RESOURCES_DIR"/default_settings.ini";
const char kDefaultWallpaperFile[] = RESOURCES_DIR"/default_wallpaper.jpg";

// File name of auto partition script.
const char kAutoPartFile[] = "auto_part.sh";

// Absolute path to oem folder.
const char kDebugOemDir[] = "/tmp/oem";
const char kUbuntuOemDir[] = "/cdrom/oem";
const char kDeepinOemDir[] = "/lib/live/mount/medium/oem";

// Filename of oem settings
const char kOemSettingsFile[] = "settings.ini";

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

bool AppendToConfigFile(const QString& key, const QString& val) {
  return AppendToConfigFile(QString("%1=\"%2\"\n").arg(key, val));
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

// Note that oem folder might not exist.
QDir GetOemDir() {
  if (g_oem_dir.isEmpty()) {
    if (QDir(kDebugOemDir).exists()) {
      g_oem_dir = kDebugOemDir;
    } else if (QDir(kUbuntuOemDir).exists()) {
      g_oem_dir = kUbuntuOemDir;
    } else {
      g_oem_dir = kDeepinOemDir;
    }
  }
  return QDir(g_oem_dir);
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

int GetSettingsInt(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toInt();
  }

  qCritical() << "GetSettingsInt() failed with key:" << key;
  return 0;
}

QString GetSettingsString(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }

  qCritical() << "GetSettingsString() failed with key:" << key;
  return QString();
}

QStringList GetSettingsStringList(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (!value.isValid()) {
    qCritical() << "GetSettingsStringList() failed with key:" << key;
    return QStringList();
  }

  return value.toString().split(';');
}

QVariant GetSettingsValue(const QString& key) {
  const QString oem_file = GetOemDir().absoluteFilePath(kOemSettingsFile);
  if (QFile::exists(oem_file)) {
    const QSettings oem_settings(oem_file , QSettings::IniFormat);
    if (oem_settings.contains(key)) {
      return oem_settings.value(key);
    }
  }

  const QSettings default_settings(kDefaultSettingsFile, QSettings::IniFormat);
  if (default_settings.contains(key)) {
    return default_settings.value(key);
  }

  qWarning() << "[SettingsManager]::getSettingsValue() Invalid key:" << key;
  return QVariant();
}

QString GetAutoPartFile() {
  const QString oem_file = GetOemDir().absoluteFilePath(kAutoPartFile);
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
  const QString oem_avatar(GetOemDir().absoluteFilePath("avatar"));
  QStringList avatars = ListImageFiles(oem_avatar);
  if (!avatars.isEmpty()) {
    return avatars;
  }

  // Then, check dde-account-faces dir.
  return ListImageFiles(GetSettingsString(kSystemInfoDdeAvatarDir));
}

QString GetDefaultAvatar() {
  const QString default_avatar(GetSettingsString(kSystemInfoDefaultAvator));
  if (!default_avatar.isEmpty() && QFile::exists(default_avatar)) {
    // Returns default avatar
    return default_avatar;
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
  return GetOemDir().absoluteFilePath("hooks");
}

QString GetVendorLogo() {
  const QString oem_file = GetOemDir().absoluteFilePath("logo.png");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  // Returns builtin logo.
  return ":/images/logo.png";
}

QString GetWindowBackground() {
  const QString oem_file = GetOemDir().absoluteFilePath("background.jpg");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  const QString in_system = GetSettingsString(kSystemInfoDdeDefaultWallpaper);
  if (QFile::exists(in_system)) {
    return in_system;
  }

  return kDefaultWallpaperFile;
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

void WriteUEFI(bool is_efi) {
  AppendToConfigFile("DI_UEFI", is_efi ? "true" : "false");
}

void WriteLocale(const QString& locale) {
  AppendToConfigFile("DI_LOCALE", locale);
}

void WriteUsername(const QString& username) {
  AppendToConfigFile("DI_USERNAME", username);
}

void WriteHostname(const QString& hostname) {
  AppendToConfigFile("DI_HOSTNAME", hostname);
}

void WritePassword(const QString& password) {
  const QString encoded_password = password.toUtf8().toBase64();
  AppendToConfigFile("DI_PASSWORD", encoded_password);
}

void WriteAvatar(const QString& avatar) {
  AppendToConfigFile("DI_AVATAR", avatar);
}

void WriteTimezone(const QString& timezone) {
  AppendToConfigFile("DI_TIMEZONE", timezone);
}

void WriteKeyboard(const QString& layout, const QString& variant) {
  AppendToConfigFile("DI_KEYBOARD_LAYOUT", layout);
  AppendToConfigFile("DI_KEYBOARD_LAYOUT_VARIANT", variant);
}

void WritePartitionInfo(const QString& root_disk,
                        const QString& root_partition,
                        const QString& boot_partition,
                        const QString& mount_points) {
  AppendToConfigFile("DI_ROOT_DISK", root_disk);
  AppendToConfigFile("DI_ROOT_PARTITION", root_partition);
  AppendToConfigFile("DI_BOOTLOADER", boot_partition);
  AppendToConfigFile("DI_MOUNTPOINTS", mount_points);
  // TODO(xushaohua): Add DI_ROOT_DISK
}

void SaveOemConfig() {
  QFile file(kInstallerOemConfigFile);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Failed to open oem file:" << kInstallerOemConfigFile;
    return;
  }
  // Read default settings
  QSettings default_settings(kDefaultSettingsFile,
                             QSettings::IniFormat);
  for (const QString& key : default_settings.allKeys()) {
    const QVariant value = default_settings.value(key);
    const QString line(QString("%1='%2'\n").arg(key).arg(value.toString()));
    file.write(line.toUtf8());
  }

  // Read oem settings
  const QString oem_file = GetOemDir().absoluteFilePath(kOemSettingsFile);
  if (QFile::exists(oem_file)) {
    QSettings oem_settings(oem_file , QSettings::IniFormat);
    for (const QString& key : oem_settings.allKeys()) {
      const QVariant value = oem_settings.value(key);
      const QString line(QString("%1='%2'\n").arg(key).arg(value.toString()));
      file.write(line.toUtf8());
    }
  }

  file.close();
}

}  // namespace installer
