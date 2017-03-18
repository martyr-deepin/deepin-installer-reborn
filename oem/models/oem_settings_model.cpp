// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "oem/models/oem_settings_model.h"

#include <QDebug>
#include <QDir>

namespace installer {

namespace {

const char kOemJsonFile[] = "oem_settings.json";
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

bool OemSettingsModel::load() {
  const QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  return LoadSettingsItems(items_,
                           kDefaultOemJsonFile,
                           kDefaultSettingsFile,
                           settings_file);
}

void OemSettingsModel::dumpItem(const OemSettingsItem& item) {
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
    qCritical() << "dumpItem() item not found" << item;
    return;
  }

  this->beginResetModel();
  items_[item_index].setValue(item.value());
  this->endResetModel();

  const QDir oem_dir(GetOemFolder());
  const QString settings_file = oem_dir.absoluteFilePath(kSettingsName);
  DumpSettingsItem(item, settings_file);
}

}  // namespace installer