/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <random>

#include "base/consts.h"
#include "service/settings_name.h"

namespace installer {

namespace {

// Absolute path to oem dir.
QString g_oem_dir;

const char kLocaleKey[] = "DI_LOCALE";

// Absolute path to installer config file.
#ifdef QT_DEBUG
const char kInstallerConfigFile[] = "/tmp/deepin-installer.conf";
#else
const char kInstallerConfigFile[] = "/etc/deepin-installer.conf";
#endif // QT_DEBUG

// Absolute path to default installer settings
const char kDefaultSettingsFile[] = RESOURCES_DIR "/default_settings.ini";
const char kDefaultWallpaperFile[] = RESOURCES_DIR "/default_wallpaper.jpg";
// File name of installer wallpaper.
const char kOemWallpaperFilename[] = "installer-background.jpg";

// File name of auto partition script.
const char kAutoPartFile[] = "auto_part.sh";
// File name of architecture specific of auto partition script.
const char kAutoPartArchSpecFile[] = "auto_part_%1.sh";

// Absolute path to oem folder.
const char kDebugOemDir[] = "/tmp/oem";
const char kUbuntuOemDir[] = "/cdrom/oem";
const char kDeepinOemDir[] = "/lib/live/mount/medium/oem";

// Filename of oem settings
const char kOemSettingsFilename[] = "settings.ini";

void AppendToConfigFile(const QString& key, const QVariant& value) {
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  settings.setValue(key, value);
}

QStringList ListAvatarFiles(const QString& dir_name) {
  QStringList result;
  QDir dir(dir_name);
  if (!dir.exists()) {
    return result;
  }

  const QStringList name_filters = { "*.png", "*.jpg" };
  const QFileInfoList info_list =
      dir.entryInfoList(name_filters, QDir::NoDotAndDotDot | QDir::Files);
  for (const QFileInfo& info : info_list) {
    // Ignores "default.png" and "guest.png".
    if (info.size() > 0 &&
        (!info.fileName().startsWith("default")) &&
        (!info.fileName().startsWith("guest"))) {
      result.append(info.absoluteFilePath());
    }
  }

  return result;
}

}  // namespace

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
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  if (settings.contains(key)) {
    return settings.value(key);
  }

  // Read default settings
  QSettings default_settings(kDefaultSettingsFile, QSettings::IniFormat);
  if (!default_settings.contains(key)) {
    qWarning() << "getSettingsValue() Invalid key:" << key;
  }
  return default_settings.value(key);
}

QString GetAutoPartFile() {
  QDir oem_dir = GetOemDir();
  QDir builtin_dir(BUILTIN_HOOKS_DIR);

  // First check existence of architecture specific file.
  const QStringList script_files = {
      oem_dir.absoluteFilePath(kAutoPartFile),
      builtin_dir.absoluteFilePath(kAutoPartFile),
  };
  for (const QString filepath : script_files) {
    if (QFile::exists(filepath)) {
      return filepath;
    }
  }

  qCritical() << "GetAutoPartFile() not partition script found!";
  return QString();
}

QStringList GetAvatars() {
  // First, check oem/ dir.
  const QString oem_avatar(GetOemDir().absoluteFilePath("avatar"));
  QStringList avatars = ListAvatarFiles(oem_avatar);
  if (!avatars.isEmpty()) {
    return avatars;
  }

  // Then, check dde-account-faces dir.
  return ListAvatarFiles(GetSettingsString(kSystemInfoDdeAvatarDir));
}

QString GetDefaultAvatar() {
  const QString default_avatar(GetSettingsString(kSystemInfoDefaultAvator));
  if (!default_avatar.isEmpty() && QFile::exists(default_avatar)) {
    // Returns default avatar
    return default_avatar;
  }

  // Pick a random avatar.
  const QStringList avatars = GetAvatars();
  if (avatars.isEmpty()) {
    return "";
  }

  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, avatars.size() - 1);

  return avatars.at(uniform_dist(e1));
}

QString GetOemHooksDir() {
  return GetOemDir().absoluteFilePath("hooks");
}

QString GetOemCheckHooksDir() {
    return GetOemDir().absoluteFilePath("check_hooks");
}

QString GetReservedUsernameFile() {
  const QString oem_file = GetOemDir().absoluteFilePath("reserved_usernames");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  // Returns default list.
  return RESOURCES_DIR "/reserved_usernames";
}

QString GetVendorLogo() {
  const QString oem_file = GetOemDir().absoluteFilePath("vendor.png");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  // Returns builtin logo.
  return ":/images/logo.svg";
}

QString GetWindowBackground() {
  const QString oem_file = GetOemDir().absoluteFilePath(kOemWallpaperFilename);
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  return kDefaultWallpaperFile;
}

bool AppendConfigFile(const QString& conf_file) {
  if (!QFile::exists(conf_file)) {
    qCritical() << "conf_file not found:" << conf_file;
    return false;
  }

  QSettings target_settings(kInstallerConfigFile, QSettings::IniFormat);
  QSettings new_settings(conf_file, QSettings::IniFormat);

  for (const QString& key : new_settings.allKeys()) {
    const QVariant value = new_settings.value(key);
    target_settings.setValue(key, value);
  }

  return true;
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
  AppendToConfigFile("DI_UEFI", is_efi);
}

QString ReadLocale() {
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  QString locale;

  // Get user-selected locale.
  locale = settings.value(kLocaleKey).toString();
  if (locale.isEmpty()) {
    // Get default locale in settings.ini.
    locale = settings.value(kSelectLanguageDefaultLocale).toString();

    if (locale.isEmpty()) {
      // Get fallback locale.
      locale = kDefaultLang;
    }
  }

  // Remove codec name from locale.
  const int dot_index = locale.indexOf('.');
  return (dot_index == -1) ? locale : locale.left(dot_index);
}

QString GetUIDefaultFont() {
  // TODO: maybe make this a configurable value in the future? since other
  // editions may have different fonts installed.
  return "Noto Sans CJK SC Regular";
}

void WriteAvatar(const QString& avatar) {
  AppendToConfigFile("DI_AVATAR", avatar);
}

void WriteHostname(const QString& hostname) {
  AppendToConfigFile("DI_HOSTNAME", hostname);
}

void WriteKeyboard(const QString& model,
                   const QString& layout,
                   const QString& variant) {
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  settings.setValue("DI_KEYBOARD_MODEL", model);
  settings.setValue("DI_LAYOUT", layout);
  settings.setValue("DI_LAYOUT_VARIANT", variant);
}

void WriteLocale(const QString& locale) {
  AppendToConfigFile("DI_LOCALE", locale);
}

void WritePassword(const QString& password) {
  const QString encoded_password = password.toUtf8().toBase64();
  AppendToConfigFile("DI_PASSWORD", encoded_password);
}

void WriteTimezone(const QString& timezone) {
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  settings.setValue("DI_TIMEZONE", timezone);
}

void WriteUsername(const QString& username) {
  AppendToConfigFile("DI_USERNAME", username);
}

void WritePartitionInfo(const QString& root_disk,
                        const QString& root_partition,
                        const QString& boot_partition,
                        const QString& mount_points) {
  qDebug() << "WritePartitionInfo()"
           << " root_disk:" << root_disk
           << ", root_partition:" << root_partition
           << ", boot_partition:" << boot_partition
           << ", mount_points:" << mount_points;
  QSettings settings(kInstallerConfigFile, QSettings::IniFormat);
  settings.setValue("DI_ROOT_DISK", root_disk);
  settings.setValue("DI_ROOT_PARTITION", root_partition);
  settings.setValue("DI_BOOTLOADER", boot_partition);
  settings.setValue("DI_MOUNTPOINTS", mount_points);
}

void WriteRequiringSwapFile(bool is_required) {
  AppendToConfigFile("DI_SWAP_FILE_REQUIRED", is_required);
}

void AddConfigFile() {
  QSettings target_settings(kInstallerConfigFile, QSettings::IniFormat);

  // Read default settings
  QSettings default_settings(kDefaultSettingsFile, QSettings::IniFormat);
  for (const QString& key : default_settings.allKeys()) {
    const QVariant value(default_settings.value(key));
    // Do not use section groups.
    target_settings.setValue(key, value);
  }

  // Read oem settings
  const QString oem_file = GetOemDir().absoluteFilePath(kOemSettingsFilename);
  if (QFile::exists(oem_file)) {
    QSettings oem_settings(oem_file , QSettings::IniFormat);
    for (const QString& key : oem_settings.allKeys()) {
      const QVariant value = oem_settings.value(key);
      target_settings.setValue(key, value);
    }
  }
}

void WriteFullDiskEncryptPassword(const QString &password)
{
    AppendToConfigFile("DI_CRYPT_PASSWD", password);
}

void WritePasswordStrong(bool strongPassword) {
    AppendToConfigFile("DI_STRONG_PASSWORD", strongPassword);
}

void WriteDisplayPort(const QString &display) {
    AppendToConfigFile("DI_DISPLAY_PORT", display);
}

void WriteGrubPassword(const QString &password) {
    AppendToConfigFile("DI_GRUB_PASSWORD", password);
}

void WriteFullDiskDeivce(const QString &deviceName)
{
    AppendToConfigFile("DI_FULLDISK_DEVICE", deviceName);
}

void WriteSwapPartitionSize(const uint size) {
    AppendToConfigFile("DI_SWAP_SIZE", size);
}

}  // namespace installer
