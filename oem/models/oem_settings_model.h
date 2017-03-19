// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
