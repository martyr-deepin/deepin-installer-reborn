// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H
#define INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H

#include <QAbstractItemModel>

namespace installer {

class OemSettingsModel : public QAbstractItemModel {
  Q_OBJECT
 public:
  explicit OemSettingsModel(QObject* parent = nullptr);

  void load();
  void save();
};

}  // namespace installer

#endif  // INSTALLER_OEM_MODELS_OEM_SETTINGS_MODEL_H
