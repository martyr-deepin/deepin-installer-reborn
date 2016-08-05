// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_FORMAT_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_FORMAT_H

#include <QString>

namespace service {

// Format filesystem.
bool FormatBtrfs(const QString& path, const QString& label);
bool FormatExt2(const QString& path, const QString& label);
bool FormatExt3(const QString& path, const QString& label);
bool FormatExt4(const QString& path, const QString& label);
bool FormatFat16(const QString& path, const QString& label);
bool FormatFat32(const QString& path, const QString& label);
bool FormatHfs(const QString& path, const QString& label);
bool FormatHfsPlus(const QString& path, const QString& label);
bool FormatJfs(const QString& path, const QString& label);
bool FormatLinuxSwap(const QString& path, const QString& label);
bool FormatNTFS(const QString& path, const QString& label);
bool FormatReiser4(const QString& path, const QString& label);
bool FormatReiserfs(const QString& path, const QString& label);
bool FormatXfs(const QString& path, const QString& label);

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_FORMAT_H
