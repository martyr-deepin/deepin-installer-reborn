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

namespace service {

namespace {

// Absolute path to oem dir.
QString g_oem_dir;

// Absolute path to installer config file.
const char kInstallerConfigFile[] = "/etc/deepin-installer.conf";

// Absolute path to global oem config file saved in system.
const char kInstallerOemConfigFile[] = "/etc/deepin-installer-oem.conf";

// File name of auto partition script.
const char kAutoPartFile[] = "auto-part.sh";

// Absolute path to oem dir in system ISO.
// Note that iso image is mounted at "/lib/live/mount/medium/".
const char kIsoOemDir[] = "/lib/live/mount/medium/oem";

// Folder name of mount point of usb storage.
const char kUsbMountDir[] = "usb_root_dir";
const char kUsbMountPoint[] = "/usb_root_dir";
const char kUsbMountOemPath[] = "/usb_root_dir/oem";

// Filename of oem settings
const char kOemSettingsFile[] = "settings.ini";

// Name of installer oem lock file.
const char kOemLockFile[] = "deepin-installer-reborn.lock";

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
  const QString line = key + "=" + val + "\n";
  return AppendToConfigFile(line);
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

// Returns a list of usb storage in system.
QStringList GetUsbStorage() {
  const QFileInfoList file_list =
      QDir("/dev/disk/by-path").entryInfoList(QDir::Files);
  QStringList result;
  for (const QFileInfo& file : file_list) {
    const QString filename = file.fileName();
    if (filename.contains(QStringLiteral("usb")) &&
        filename.contains(QStringLiteral("part"))) {
      result.append(QFileInfo(file).canonicalFilePath());
    }
  }
  return result;
}

// TODO(xushaohua): Scan usb devices explicitly.
bool ScanLatestOemDirInUsbStorage() {
  QDir root_dir("/");
  if (root_dir.exists(kUsbMountDir)) {
    // Umount any devices, ignoring error message.
    (void)umount(kUsbMountPoint);
  } else if (!root_dir.mkdir(kUsbMountDir)) {
    qCritical() << "Failed to create:" << kUsbMountDir;
    return false;
  }

  QDateTime latest_timestamp = QDateTime::fromTime_t(0);
  QString latest_device;
  const QStringList usb_devices = GetUsbStorage();
  qDebug() << "usb devices:" << usb_devices;
  if (usb_devices.isEmpty()) {
    // If no usb device is found, use ISO folder instead.
    g_oem_dir = kIsoOemDir;
    return true;
  }

  for (const QString& usb_device : usb_devices) {
    qDebug() << "Scanning:" << usb_device;
    // mount usb device
    const char* device_path = usb_device.toStdString().c_str();

    // Filesystem type of usb device is hard-coded!
    if (mount(device_path, kUsbMountPoint, "vfat", MS_NOATIME, NULL) != 0) {
      qCritical() << "Failed to mount:" << usb_device << strerror(errno);
      return false;
    }
    // check folder and file exists
    QDir oem_dir(kUsbMountOemPath);
    if (oem_dir.exists() && oem_dir.exists(kOemLockFile)) {
      qDebug() << "lock file found:" << usb_device;
      // compare timestamp
      QFileInfo info(oem_dir.absoluteFilePath(kOemLockFile));
      const QDateTime timestamp = info.lastModified();
      if (timestamp > latest_timestamp) {
        latest_timestamp = timestamp;
        latest_device = usb_device;
      }
    }

    // umount usb device
    if (umount(kUsbMountPoint) != 0) {
      qCritical() << "Failed to umount:" << usb_device;
      return false;
    }
  }

  // compare with ISO device
  QDir iso_oem_dir(kIsoOemDir);
  if (iso_oem_dir.exists() && iso_oem_dir.exists(kOemLockFile)) {
    QFileInfo info(iso_oem_dir.absoluteFilePath(kOemLockFile));
    const QDateTime timestamp = info.lastModified();
    if (timestamp > latest_timestamp) {
      g_oem_dir = kIsoOemDir;
      return true;
    }
  }

  if (!latest_device.isEmpty()) {
    // Mount that usb device.
    if (mount(latest_device.toStdString().c_str(), kUsbMountPoint,
              "vfat", MS_NOATIME, NULL) != 0) {
      qCritical() << "Failed to mount:" << latest_device << strerror(errno);
      return false;
    }
    qDebug() << "Use usb device:" << latest_device;
    g_oem_dir = kUsbMountOemPath;
    return true;
  }

  return false;
}

QDir GetOemDir() {
  if (g_oem_dir.isEmpty()) {
    if (!ScanLatestOemDirInUsbStorage()) {
      qCritical() << "ScanLatestOemDirInUsbStorage() failed!";
      // Fallback to ISO folder.
      g_oem_dir = kIsoOemDir;
    } else {
      qDebug() << "Oem dir:" << g_oem_dir;
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

QString GetSettingsString(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }

  qCritical() << "GetSettingsString() failed with key:" << key;
  return QStringLiteral("");
}

QVariant GetSettingsValue(const QString& key) {
  const QString oem_file = GetOemDir().absoluteFilePath(kOemSettingsFile);
  if (QFile::exists(oem_file)) {
    QSettings oem_settings(oem_file , QSettings::IniFormat);
    if (oem_settings.contains(key)) {
      return oem_settings.value(key);
    }
  }

  QSettings default_settings(QStringLiteral(":/resources/default-settings.ini"),
                             QSettings::IniFormat);
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
  // Return user specified avatar.
  if (GetSettingsBool(kSystemInfoUseDefaultAvator)) {
    return GetSettingsString(kSystemInfoDefaultAvator);
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
  return GetOemDir().absoluteFilePath(QStringLiteral("hooks"));
}

QString GetOverlayFilesystemDir() {
  return GetOemDir().absoluteFilePath(QStringLiteral("overlay"));
}

QString GetVendorLogo() {
  const QString oem_file = GetOemDir().absoluteFilePath("logo.png");
  if (QFile::exists(oem_file)) {
    return oem_file;
  }

  // Returns builtin logo.
  return QStringLiteral(":/images/logo.png");
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

void WritePartitionInfo(const QString& root, const QString& mount_points) {
  AppendToConfigFile("DI_ROOT_PARTITION", root);
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
  QSettings default_settings(QStringLiteral(":/resources/default-settings.ini"),
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

}  // namespace service
