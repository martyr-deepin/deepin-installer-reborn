// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H
#define DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H

#include <QtCore/QtGlobal>

namespace ui {

// Convert |size| in byte to gigbyte.
int ToGigByte(qint64 size);

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H
