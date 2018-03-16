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

#ifndef INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H
#define INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H

#include <QAbstractListModel>

#include "oem/models/oem_settings_item.h"

namespace installer {

// Read and write oem settings.
class OemSettingsModel : public QAbstractListModel {
  Q_OBJECT
 public:
  explicit OemSettingsModel(QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get oem item at |index|.
  OemSettingsItem getItem(const QModelIndex& index) const;

 public slots:
  // Create $HOME/oem folder and sub-folders.
  // Returns false if failed to create folders.
  bool createOemFolders();

  // Dump all items in item list to file.
  // Call this slot before program exits.
  void dumpItems();

  // Load default settings from system, and then load settings in
  // $HOME/oem folder if exists.
  bool load();

  // Update |item| in item list
  void updateItem(const OemSettingsItem& item);

 private:
  OemSettingsItems items_;
};

}  // namespace installer

#endif  // INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H
