// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/oem/settings_model.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

#include "service/settings_name.h"

namespace installer {

namespace {

const char kSettingsName[] = "settings.ini";

const char kDefaultSettingsFile[] = RESOURCES_DIR "/default_settings.ini";

// Returns absolute path to $HOME/oem/ folder.
QString GetOemFolder() {
  return QDir::home().absoluteFilePath("oem");
}

QVariant GetSettingsValue(const QString& key);

bool GetSettingsBool(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toBool();
  }

  qCritical() << "GetSettingsBool() failed with key:" << key;
  return false;
}

//int GetSettingsInt(const QString& key) {
//  const QVariant value = GetSettingsValue(key);
//  if (value.isValid()) {
//    return value.toInt();
//  }
//
//  qCritical() << "GetSettingsInt() failed with key:" << key;
//  return 0;
//}

QString GetSettingsString(const QString& key) {
  const QVariant value = GetSettingsValue(key);
  if (value.isValid()) {
    return value.toString();
  }

  qCritical() << "GetSettingsString() failed with key:" << key;
  return QString();
}

//QStringList GetSettingsStringList(const QString& key) {
//  const QVariant value = GetSettingsValue(key);
//  if (!value.isValid()) {
//    qCritical() << "GetSettingsStringList() failed with key:" << key;
//    return QStringList();
//  }
//
//  return value.toString().split(';');
//}

QVariant GetSettingsValue(const QString& key) {
  QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings settings(settings_file, QSettings::IniFormat);
  const QVariant value(settings.value(key));
  if (!value.isValid()) {
    qWarning() << "getSettingsValue() Invalid key:" << key;
  }
  return value;
}

// Write settings to file.
void SetSettingsValue(const QString& key, const QVariant& value) {
  QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings settings(settings_file, QSettings::IniFormat);
  settings.setValue(key, value);
}

}  // namespace

SettingsModel::SettingsModel(QObject* parent) : QObject(parent) {
  this->setObjectName("settings_model");
}

bool SettingsModel::initOemFolder() {
  QDir oem_dir(GetOemFolder());

  if (!oem_dir.exists()) {
    if (!oem_dir.mkpath(".")) {
      qWarning() << "Failed to create oem dir" << oem_dir.absolutePath();
      return false;
    }
  }

  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  QSettings oem_settings(settings_file, QSettings::IniFormat);
  QSettings default_settings(kDefaultSettingsFile, QSettings::IniFormat);
  for (const QString& key : default_settings.allKeys()) {
    // Merge default settings. Do not use section groups.
    if (!oem_settings.contains(key)) {
      const QVariant value(default_settings.value(key));
      oem_settings.setValue(key, value);
    }
  }

  return true;
}

bool SettingsModel::skipDiskSpacePage() const {
  return GetSettingsBool(kSkipDiskSpaceInsufficientPage);
}

bool SettingsModel::skipVirtualMachinePage() const {
  return GetSettingsBool(kSkipVirtualMachinePage);
}

bool SettingsModel::skipLanguagePage() const {
  return GetSettingsBool(kSkipSelectLanguagePage);
}

bool SettingsModel::skipTableWarningPage() const {
  return GetSettingsBool(kSkipPartitionTableWarningPage);
}

bool SettingsModel::skipSystemInfoPage() const {
  return GetSettingsBool(kSkipSystemInfoPage);
}

bool SettingsModel::skipPartitionPage() const {
  return GetSettingsBool(kSkipPartitionPage);
}

QString SettingsModel::defaultLocale() const {
  return GetSettingsString(kSelectLanguageDefaultLocale);
}

QString SettingsModel::vendorName() const {
  return GetSettingsString(kSystemInfoVendorName);
}

bool SettingsModel::lockUsername() const {
  return GetSettingsBool(kSystemInfoLockUsername);
}

bool SettingsModel::lockHostname() const {
  return GetSettingsBool(kSystemInfoLockHostname);
}

bool SettingsModel::lockPassword() const {
  return GetSettingsBool(kSystemInfoLockPassword);
}

QString SettingsModel::defaultUsername() const {
  return GetSettingsString(kSystemInfoDefaultUsername);
}

QString SettingsModel::defaultHostname() const {
  return GetSettingsString(kSystemInfoDefaultHostname);
}

QString SettingsModel::defaultPassword() const {
  return GetSettingsString(kSystemInfoDefaultPassword);
}

void SettingsModel::setSkipDiskSpacePage(bool checked) {
  SetSettingsValue(kSkipDiskSpaceInsufficientPage, checked);
}

void SettingsModel::setSkipVirtualMachinePage(bool checked) {
  SetSettingsValue(kSkipVirtualMachinePage, checked);
}

void SettingsModel::setSkipLanguagePage(bool checked) {
  SetSettingsValue(kSkipSelectLanguagePage, checked);
}

void SettingsModel::setSkipTableWarningPage(bool checked) {
  SetSettingsValue(kSkipPartitionTableWarningPage, checked);
}

void SettingsModel::setSkipSystemInfoPage(bool checked) {
  SetSettingsValue(kSkipSystemInfoPage, checked);
}

void SettingsModel::setSkipPartitionPage(bool checked) {
  SetSettingsValue(kSkipPartitionPage, checked);
}

void SettingsModel::setDefaultLocale(const QString& locale) {
  SetSettingsValue(kSelectLanguageDefaultLocale, locale);
}

void SettingsModel::setVendorName(const QString& vendor) {
  SetSettingsValue(kSystemInfoVendorName, vendor);
}

void SettingsModel::setLockUsername(bool checked) {
  SetSettingsValue(kSystemInfoLockUsername, checked);
}

void SettingsModel::setLockHostname(bool checked) {
  SetSettingsValue(kSystemInfoLockHostname, checked);
}

void SettingsModel::setLockPassword(bool checked) {
  SetSettingsValue(kSystemInfoLockPassword, checked);
}

void SettingsModel::setDefaultUsername(const QString& username) {
  SetSettingsValue(kSystemInfoDefaultUsername, username);
}

void SettingsModel::setDefaultHostname(const QString& hostname) {
  SetSettingsValue(kSystemInfoDefaultHostname, hostname);
}

void SettingsModel::setDefaultPassword(const QString& password) {
  SetSettingsValue(kSystemInfoDefaultPassword, password);
}

}  // namespace installer