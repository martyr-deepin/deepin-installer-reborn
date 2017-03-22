// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H

#include <QList>

namespace installer {

enum class AdvancedValidateState {
  BootFsInvalid,  // Filesystem used for /boot is not in supported fs list.
  BootPartNumberInvalid,  // Partition for /boot is not the first partition.
  BootTooSmall,
  EfiMissing,
  EfiTooSmall,
  RootMissing,
  RootTooSmall,
  Ok,
};
typedef QList <AdvancedValidateState> AdvancedValidateStates;

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H
