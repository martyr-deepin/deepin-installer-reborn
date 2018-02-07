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

#include "oem/models/oem_settings_model.h"

#include <QDebug>
#include <QDir>

namespace installer {

namespace {

const char kBeforeChroot[] = "before_chroot";
const char kInChroot[] = "in_chroot";
const char kAfterChroot[] = "after_chroot";

const char kSettingsName[] = "settings.ini";
const char kDefaultSettingsFile[] = RESOURCES_DIR "/default_settings.ini";
const char kDefaultOemJsonFile[] = RESOURCES_DIR "/oem_settings.json";

// Returns absolute path to $HOME/oem/ folder.
QString GetOemFolder() {
  return QDir::home().absoluteFilePath("oem");
}

}  // namespace

OemSettingsModel::OemSettingsModel(QObject* parent)
    : QAbstractListModel(parent),
      items_() {
  this->setObjectName("oem_settings_model");
}

QVariant OemSettingsModel::data(const QModelIndex& index, int role) const {
  const int row = index.row();

  if (!index.isValid() || row < 0 || row >= items_.length()) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
    return items_.at(row).title();
  } else if (role == Qt::ToolTipRole) {
    return items_.at(row).description();
  } else {
    return QVariant();
  }
}

int OemSettingsModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return items_.length();
}

OemSettingsItem OemSettingsModel::getItem(const QModelIndex& index) const {
  if (index.isValid()) {
    return items_.at(index.row());
  } else {
    return OemSettingsItem();
  }
}

bool OemSettingsModel::createOemFolders() {
  QDir oem_dir(GetOemFolder());
  if (!oem_dir.mkpath(".")) {
    qCritical() << "Failed to create $HOME/oem folder";
    return false;
  }
  if (!oem_dir.mkpath("deb")) {
    qCritical() << "Failed to create $HOME/oem/deb folder";
    return false;
  }
  QDir hooks_dir(oem_dir.absoluteFilePath("hooks"));
  if (!hooks_dir.mkpath(".")) {
    qCritical() << "Failed to create $HOME/oem/hooks folder";
    return false;
  }
  if (!hooks_dir.mkpath(kBeforeChroot)) {
    qCritical() << "Failed to create " << kBeforeChroot;
    return false;
  }
  if (!hooks_dir.mkpath(kInChroot)) {
    qCritical() << "Failed to create" << kInChroot;
    return false;
  }
  if (!hooks_dir.mkpath(kAfterChroot)) {
    qCritical() << "Failed to create" << kAfterChroot;
    return false;
  }
  return true;
}

void OemSettingsModel::dumpItems() {
  // If items_ is empty, do nothing.
  if (items_.isEmpty()) {
    return;
  }
  const QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  DumpSettingsItems(items_, settings_file);
}

bool OemSettingsModel::load() {
  const QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  this->beginResetModel();
  const bool ok = LoadSettingsItems(items_,
                                    kDefaultOemJsonFile,
                                    kDefaultSettingsFile,
                                    settings_file);
  this->endResetModel();
  return ok;
}

void OemSettingsModel::updateItem(const OemSettingsItem& item) {
  // Do nothing if item list is empty.
  if (items_.isEmpty()) {
    qWarning() << "item list is empty";
    return;
  }

  int item_index = -1;
  for (item_index = 0; item_index < items_.length(); ++item_index) {
    if (item.name() == items_.at(item_index).name()) {
      break;
    }
  }
  if (item_index >= items_.length() || item_index < 0) {
    qCritical() << "updateItem() item not found" << item;
    return;
  }

  // No need to reset model.
//  this->beginResetModel();
  items_[item_index].setValue(item.value());
//  this->endResetModel();
}

}  // namespace installer