// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/libparted_util.h"

#include <QDebug>

#include "base/command.h"

namespace partman {

bool CommitDiskChanges(PedDisk* lp_disk) {
  bool success = static_cast<bool>(ped_disk_commit_to_dev(lp_disk));
  if (success) {
    success = static_cast<bool>(ped_disk_commit_to_os(lp_disk));
  }
  return success;
}

bool DeletePartition(const Partition& partition) {
  bool ok = false;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  if (GetDeviceAndDisk(partition.path, lp_device, lp_disk)) {
    PedPartition* lp_partition = NULL;
    if (partition.type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition.getSector());
    }
    if (lp_partition) {
      ok = bool(ped_disk_delete_partition(lp_disk, lp_partition));
    }

    if (ok) {
      ok = CommitDiskChanges(lp_disk);
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  }

  return ok;
}

void DestroyDeviceAndDisk(PedDevice*& lp_device, PedDisk*& lp_disk) {
  if (lp_device) {
    ped_device_destroy(lp_device);
    lp_device = NULL;
  }

  if (lp_disk) {
    ped_disk_destroy(lp_disk);
    lp_disk = NULL;
  }
}

bool FlushDevice(PedDevice* lp_device) {
  bool success = false;
  if (ped_device_open(lp_device)) {
    success = static_cast<bool>(ped_device_sync(lp_device));
    ped_device_close(lp_device);
  }
  return success;
}

bool GetDeviceAndDisk(const QString& path,
                      PedDevice*& lp_device,
                      PedDisk*& lp_disk) {
  lp_device = ped_device_get(path.toLatin1().data());
  if (lp_device) {
    lp_disk = ped_disk_new(lp_device);
    if (lp_disk != NULL) {
      return true;
    } else {
      DestroyDeviceAndDisk(lp_device, lp_disk);
      return false;
    }
  } else {
    return false;
  }
}

bool SetPartitionType(const Partition& partition) {
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  bool ok = false;
  if (GetDeviceAndDisk(partition.path, lp_device, lp_disk)) {

    QString fs_name = GetFsTypeName(partition.fs);
    // Default fs is Linux (83).
    if (fs_name.isEmpty()) {
      qWarning() << "SetPartitionType: no fs is specified, use default!";
      fs_name = GetFsTypeName(FsType::Ext2);
    }

    PedFileSystemType* lp_fs_type =
        ped_file_system_type_get(fs_name.toLatin1().data());

    PedPartition* lp_partition =
        ped_disk_get_partition_by_sector(lp_disk, partition.getSector());

    if (lp_fs_type && lp_partition) {
      ok = bool(ped_partition_set_system(lp_partition, lp_fs_type));
    }

    // Set ESP flag
    if (ok && partition.fs == FsType::EFI) {
      ok = bool(ped_partition_set_flag(lp_partition, PED_PARTITION_ESP, 1));
    }

    if (ok) {
      ok = CommitDiskChanges(lp_disk);
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  }

  return ok;
}

void SettleDevice(int timeout) {
  base::SpawnCmd("udevadm", {"settle", QString("--timeout=%1").arg(timeout)});
}

}  // namespace partman