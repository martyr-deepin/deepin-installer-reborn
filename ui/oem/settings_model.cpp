// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/oem/settings_model.h"

namespace installer {

SettingsModel::SettingsModel(QObject* parent) : QObject(parent) {
  this->setObjectName("settings_model");
}

bool SettingsModel::skipDiskSpacePage() const {
  return false;
}

bool SettingsModel::skipVirtualMachinePage() const {
  return false;
}

bool SettingsModel::skipLanguagePage() const {
  return false;
}

bool SettingsModel::skipSystemInfoPage() const {
  return false;
}

bool SettingsModel::skipPartitionPage() const {
  return false;
}

bool SettingsModel::useDefaultLocale() const {
  return false;
}

QStringList SettingsModel::languageList() const {
  return QStringList();
}

void SettingsModel::setSkipDiskSpacePage(bool checked) {
  Q_UNUSED(checked);
}

void SettingsModel::setSkipVirtualMachinePage(bool checked) {
  Q_UNUSED(checked);

}

void SettingsModel::setSkipLanguagePage(bool checked) {
  Q_UNUSED(checked);

}

void SettingsModel::setSkipSystemInfoPage(bool checked) {
  Q_UNUSED(checked);

}

void SettingsModel::setSkipPartitionPage(bool checked) {
  Q_UNUSED(checked);

}

void SettingsModel::setUseDefaultLocale(bool checked) {
  Q_UNUSED(checked);

}

}  // namespace installer