// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H

#include <QObject>

#include "service/partition_manager_structs.h"

namespace service {

class PartitionManager : public QObject {
  Q_OBJECT

 public:
  explicit PartitionManager(QObject* parent = nullptr);

 signals:
  void refreshDevices();
  void devicesRefreshed();

  // Run oem/auto-part.sh script.
  void autoPart();
  // Emitted after oem/auto-part.sh script is executed and exited.
  // |ok| is true if that script exited 0.
  void autoPartDone(bool ok);

  void manualPart();
  void manualPartDone(bool ok);

 private:
  void initConnections();

 private slots:
  void doRefreshDevices();
  void doAutoPart();
  void doManualPart();
};

QString GetFsTypeName(FsType fs_type);
FsType GetFsTypeByName(const QString& name);

// Check if EFI feature is enabled in this machine.
bool IsEfiEnabled();

// Get maximum device size, by parsing /proc/partitions.
qint64 GetMaximumDeviceSize();

// Returns partition table type of the first disk device.
PartitionTableType GetPrimaryDiskPartitionTable();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
