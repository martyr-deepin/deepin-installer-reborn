// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H

#include <QObject>
#include <QList>

namespace service {

enum class PartitionTableType {
  Empty,  // Raw disk has empty partition table type.
  GPT,
  MsDos,
  Others,  // Not supported partition types.
  Unknown,
};

enum class FsType {
  Empty,
  Btrfs,
  EFI,
  Ext2,
  Ext3,
  Ext4,
  Fat16,
  Fat32,
  Hfs,
  HfsPlus,
  Jfs,
  LinuxSwap,
  LVM2PV,
  NTFS,
  Others,
  Reiser4,
  Reiserfs,
  Xfs,
  Unknown,
};

struct Partition {
  // File system
  QString fs;  // file system type.
  qint64 freespace;
  qint64 length;
  QString label;
  QString uuid;
  QStringList mounts;

  // Partition
  QString path;
  qint64 first_sector;
  qint64 last_sector;
  qint64 total_sectors;
  QString name;
  QStringList flags;
  QString os;
};

struct Device {
  QString model;  // Human readable device name (manufacture).
  QString path;
  PartitionTableType table;
  qint64 freespace;
  qint64 length;  // total sectors / disk capacity.
  int heads;
  int sectors;
  int cylinders;
  qint64 sector_size;
  QList<Partition> partitions;
};

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

// Check if EFI feature is enabled in this machine.
bool IsEfiEnabled();

// Get maximum device size, by parsing /proc/partitions.
qint64 GetMaximumDeviceSize();

// Returns partition table type of the first disk device.
PartitionTableType GetPrimaryDiskPartitionTable();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
