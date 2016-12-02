// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/libparted_util.h"

#include <QDebug>

#include "base/command.h"

namespace installer {

namespace {

// Get |fs| name used in libparted.
QString GetPedFsName(FsType fs) {
  QString fs_name;
  if (fs == FsType::EFI) {
    // Handles EFI -> fat32
    fs_name = GetFsTypeName(FsType::Fat32);
  } else {
    fs_name = GetFsTypeName(fs);
  }
  // Default fs is Linux (83).
  if (fs_name.isEmpty()) {
    qWarning() << "SetPartitionType: no fs is specified, use default!";
    fs_name = GetFsTypeName(FsType::Ext2);
  }
  return fs_name;
}

}  // namespace

bool Commit(PedDisk* lp_disk) {
  bool success = static_cast<bool>(ped_disk_commit_to_dev(lp_disk));
  if (success) {
    success = static_cast<bool>(ped_disk_commit_to_os(lp_disk));
  }

  SettleDevice(3);
  return success;
}

bool CreatePartition(const Partition& partition) {
  qDebug() << "CreatePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  if (GetDeviceAndDisk(partition.device_path, lp_device, lp_disk)) {
    PedPartitionType type;

    switch (partition.type) {
      case PartitionType::Normal: {
        type = PED_PARTITION_NORMAL;
        break;
      }
      case PartitionType::Logical: {
        type = PED_PARTITION_LOGICAL;
        break;
      }
      case PartitionType::Extended: {
        type = PED_PARTITION_EXTENDED;
        break;
      }
      default: {
        type = PED_PARTITION_FREESPACE;
        break;
      }
    }

    PedFileSystemType* fs_type = NULL;
    if (partition.type != PartitionType::Extended) {
      const QString fs_name = GetPedFsName(partition.fs);
      fs_type = ped_file_system_type_get(fs_name.toStdString().c_str());
    }

    PedPartition* lp_partition = ped_partition_new(lp_disk,
                                                   type,
                                                   fs_type,
                                                   partition.start_sector,
                                                   partition.end_sector);
    if (lp_partition) {
      PedConstraint* constraint = NULL;
      PedGeometry* geom = ped_geometry_new(lp_device,
                                           partition.start_sector,
                                           partition.getSectorLength());
      if (geom) {
        constraint = ped_constraint_exact(geom);
      } else {
        qCritical() << "CreatePartition() geom is NULL";
      }
      if (constraint) {
        // TODO(xushaohua): Change constraint.min_size.
        ok = bool(ped_disk_add_partition(lp_disk, lp_partition, constraint));
        if (ok) {
          ok = Commit(lp_disk);
        } else {
          qCritical() << "CreatePartition() ped_disk_add_partition() failed";
        }
        ped_geometry_destroy(geom);
        ped_constraint_destroy(constraint);
      } else {
        qCritical() << "CreatePartition() constraint is NULL";
      }
    } else {
      qCritical() << "CreatePartition() ped_partition_new() returns NULL"
                  << partition.path;
    }
    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "CreatePartition() failed to get lp disk object"
                << partition.path;
  }

  return ok;
}

bool DeletePartition(const Partition& partition) {
  qDebug() << "DeletePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  if (GetDeviceAndDisk(partition.device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition = NULL;
    if (partition.type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition.getSector());
    }
    if (lp_partition) {
      ok = bool(ped_disk_delete_partition(lp_disk, lp_partition));
    } else {
      qCritical() << "DeletePartition() lp_partition is NULL";
    }

    if (ok) {
      ok = Commit(lp_disk);
    } else {
      qCritical() << "DeletePartition ped_disk_delete_partition() failed";
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "DeletePartition() failed to get disk object"
                << partition.path;
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

bool GetDeviceAndDisk(const QString& device_path,
                      PedDevice*& lp_device,
                      PedDisk*& lp_disk) {
  lp_device = ped_device_get(device_path.toLatin1().data());
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

QString GetPartitionPath(PedPartition* lp_partition) {
  // Result of ped_partition_get_path() need to be freed by hand.
  char* lp_path = ped_partition_get_path(lp_partition);
  QString path;
  if (lp_path != NULL) {
    path = lp_path;
    free(lp_path);
  }
  return path;
}

bool ResizeMovePartition(const Partition& partition) {
  qDebug() << "ResizeMovePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  if (GetDeviceAndDisk(partition.device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition = NULL;
    if (partition.type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition.getSector());
    }
    if (lp_partition) {
      PedGeometry* geom = ped_geometry_new(lp_device, partition.start_sector,
                                           partition.getSectorLength());
      PedConstraint* constraint = NULL;
      if (geom) {
        constraint = ped_constraint_exact(geom);
      }
      if (constraint) {
        ok = bool(ped_disk_set_partition_geom(lp_disk, lp_partition, constraint,
                                              partition.start_sector,
                                              partition.end_sector));
        if (ok) {
          ok = Commit(lp_disk);
        }
        ped_geometry_destroy(geom);
        ped_constraint_destroy(constraint);
      }
    }
    DestroyDeviceAndDisk(lp_device, lp_disk);
  }
  return ok;
}

bool SetPartitionFlag(const Partition& partition,
                      PedPartitionFlag flag,
                      bool is_set) {
  qDebug() << "SetPartitionFlag()" << partition << flag << is_set;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  bool ok = false;
  if (GetDeviceAndDisk(partition.device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition =
        ped_disk_get_partition_by_sector(lp_disk, partition.getSector());
    if (lp_partition) {
      ok = bool(ped_partition_set_flag(lp_partition, flag, is_set ? 1 : 0));
    }

    if (ok) {
      ok = Commit(lp_disk);
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  }
  return ok;
}

bool SetPartitionFlags(const Partition& partition) {
  for (PartitionFlag flag : partition.flags) {
    if (!SetPartitionFlag(partition,
                          static_cast<PedPartitionFlag>(flag),
                          true)) {
      return false;
    }
  }
  return true;
}

bool SetPartitionType(const Partition& partition) {
  qDebug() << "SetPartitionType:" << partition;
  PedDevice* lp_device = NULL;
  PedDisk* lp_disk = NULL;
  bool ok = false;
  if (GetDeviceAndDisk(partition.device_path, lp_device, lp_disk)) {
    const QString fs_name = GetPedFsName(partition.fs);

    PedFileSystemType* fs_type =
        ped_file_system_type_get(fs_name.toStdString().c_str());

    PedPartition* lp_partition =
        ped_disk_get_partition_by_sector(lp_disk, partition.getSector());

    if (fs_type && lp_partition) {
      ok = bool(ped_partition_set_system(lp_partition, fs_type));
    } else {
      qCritical() << "SetPartitionType() ped_disk_get_partition_by_sector() "
                  << "failed";
    }

    if (ok) {
      ok = Commit(lp_disk);
    } else {
      qCritical() << "SetPartitionType() ped_partition_set_system() failed";
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "SetPartitionType() failed to get disk object:"
                << partition.path;
  }

  return ok;
}

void SettleDevice(int timeout) {
  SpawnCmd("udevadm", {"settle", QString("--timeout=%1").arg(timeout)});
}

}  // namespace installer