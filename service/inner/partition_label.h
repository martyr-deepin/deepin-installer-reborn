// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_LABEL_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_LABEL_H

#include <QString>

// Read partition label.
namespace service {

QString ReadBtrfsLabel(const QString& path);
QString ReadExt2Label(const QString& path);
QString ReadFat16Label(const QString& path);
QString ReadHfsLabel(const QString& path);
QString ReadJfsLabel(const QString& path);
QString ReadLinuxSwapLabel(const QString& path);
QString ReadNTFSLabel(const QString& path);
QString ReadReiser4Label(const QString& path);
QString ReadReiserfsLabel(const QString& path);
QString ReadXfsLabel(const QString& path);

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_LABEL_H
