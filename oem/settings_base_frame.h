// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_SETTINGS_BASE_FRAME_H
#define INSTALLER_UI_OEM_SETTINGS_BASE_FRAME_H

#include <QFrame>

namespace installer {

class SettingsBaseModel;

class SettingsBaseFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SettingsBaseFrame(QWidget* parent = nullptr);

 protected:
  SettingsBaseModel* model_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_SETTINGS_BASE_FRAME_H
