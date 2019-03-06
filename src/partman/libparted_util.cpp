/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "partman/libparted_util.h"

#include <QDebug>
#include <QFileInfo>

#include "base/command.h"

namespace installer {

namespace {

void PrintPedConstraintInfo(PedConstraint *constraint)
{
  qDebug() << "constraint: {" << "\n"
  << "    start_align: " <<  constraint->start_align->offset << "\n"
  << "    end_align: " <<  constraint->end_align->offset << "\n"
  << "    start_range: {" << "\n"
  << "        " << constraint->start_range->start << "\n"
  << "        " << constraint->start_range->end << "\n"
  << "    }" << "\n"
  << "    end_range: {" << "\n"
  << "        " << constraint->end_range->start << "\n"
  << "        " << constraint->end_range->end << "\n"
  << "    }";
}

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

// Get ped file system type pointer which is used by |partition|.
PedFileSystemType* GetPedFsType(const Partition::Ptr partition) {
  if (partition->type != PartitionType::Extended) {
    PedFileSystemType* fs_type = nullptr;
    const QString fs_name = GetPedFsName(partition->fs);
    fs_type = ped_file_system_type_get(fs_name.toLocal8Bit().constData());
    if (fs_type == NULL) {
      // Fallback to default filesystem type.
      fs_type = ped_file_system_type_get("ext2");
    }
    return fs_type;
  } else {
    return NULL;
  }
}

}  // namespace

bool Commit(PedDisk* lp_disk) {
  const bool success = (bool)ped_disk_commit(lp_disk);

  SettleDevice(5);
  return success;
}

bool CommitUdevEvent(const QString& dev_path) {
  SettleDevice(5);
  for (int i = 0; !QFileInfo::exists(dev_path) && i < 50; ++i) {
    // Wait for 10ms.
    struct timespec rqtp = { 0, 10 * 1000 * 1000 };
    (void) nanosleep(&rqtp, NULL);
  }

  return QFileInfo::exists(dev_path);
}

bool CreatePartition(const Partition::Ptr partition) {
  qDebug() << "CreatePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    PedPartitionType type;

    switch (partition->type) {
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

    PedFileSystemType* fs_type = GetPedFsType(partition);
    PedPartition* lp_partition = ped_partition_new(lp_disk,
                                                   type,
                                                   fs_type,
                                                   partition->start_sector,
                                                   partition->end_sector);
    if (lp_partition) {
      PedConstraint* constraint = nullptr;
      PedGeometry* geom = ped_geometry_new(lp_device,
                                           partition->start_sector,
                                           partition->getSectorLength());
      if (geom) {
        // Create a relatively loose constraint,
        // leaving other things to libparted.
        constraint = ped_constraint_exact(geom);
      } else {
        qCritical() << "CreatePartition() geom is nullptr";
      }

      if (!constraint) {
        // try again for ped_constraint_new_from_max when ped_constraint_exact failed.
        constraint = ped_constraint_new_from_max(geom);
        qWarning() << "ped_constraint_exact failed";
      }

      if (constraint) {
        // TODO(xushaohua): Change constraint.min_size.
        // PrintPedConstraintInfo(constraint);
        ok = bool(ped_disk_add_partition(lp_disk, lp_partition, constraint));
        if (ok) {
          ok = Commit(lp_disk);
        } else {
          qCritical() << "CreatePartition() ped_disk_add_partition() failed";
        }
        ped_geometry_destroy(geom);
        ped_constraint_destroy(constraint);
      } else {
        qCritical() << "CreatePartition() constraint is nullptr";
      }
    } else {
      qCritical() << "CreatePartition() ped_partition_new() returns nullptr"
                  << partition;
    }
    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "CreatePartition() failed to get lp disk object"
                << partition;
  }

  return ok;
}

bool CreatePartitionTable(const QString& device_path,
                          PartitionTableType table) {
  PedDiskType* disk_type = NULL;
  switch (table) {
    case PartitionTableType::GPT: {
      disk_type = ped_disk_type_get(kPartitionTableGPT);
      break;
    }
    case PartitionTableType::MsDos: {
      disk_type = ped_disk_type_get(kPartitionTableMsDos);
      break;
    }

    default: {
      qCritical() << "CreatePartitionTable() Unsupported partition table.";
      return false;
    }
  }

  if (disk_type == NULL) {
    qCritical() << "CreatePartitionTable() Failed to get disk type";
    return false;
  }

  PedDevice* lp_device = NULL;
  lp_device = ped_device_get(device_path.toStdString().c_str());

  if (lp_device != NULL) {
    PedDisk* lp_disk = NULL;
    // Create a new device table but not commit changes to device.
    lp_disk = ped_disk_new_fresh(lp_device, disk_type);
    if (lp_disk != NULL) {
      // Commit explicitly. Or else newly created partition table is only
      // in memory.
      Commit(lp_disk);
      DestroyDeviceAndDisk(lp_device, lp_disk);
      return true;
    } else {
      qCritical() << "CreatePartitionTable() Failed to create new disk"
                  << device_path;
      DestroyDevice(lp_device);
      return false;
    }
  } else {
    qCritical() << "CreatePartitionTable() Failed to get device at"
                << device_path;
    return false;
  }
}

bool DeletePartition(const Partition::Ptr partition) {
  qDebug() << "DeletePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition = nullptr;
    if (partition->type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition->getSector());
    }
    if (lp_partition) {
      ok = bool(ped_disk_delete_partition(lp_disk, lp_partition));
    } else {
      qCritical() << "DeletePartition() lp_partition is nullptr";
    }

    if (ok) {
      ok = Commit(lp_disk);
    } else {
      qCritical() << "DeletePartition ped_disk_delete_partition() failed";
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "DeletePartition() failed to get disk object"
                << partition;
  }

  return ok;
}

void DestroyDevice(PedDevice* lp_device) {
  if (lp_device) {
    ped_device_destroy(lp_device);
    lp_device = nullptr;
  }
}

void DestroyDeviceAndDisk(PedDevice*& lp_device, PedDisk*& lp_disk) {
  if (lp_device) {
    ped_device_destroy(lp_device);
    lp_device = nullptr;
  }

  if (lp_disk) {
    ped_disk_destroy(lp_disk);
    lp_disk = nullptr;
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
  lp_device = ped_device_get(device_path.toStdString().c_str());
  if (lp_device) {
    lp_disk = ped_disk_new(lp_device);
    if (lp_disk != nullptr) {
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
  if (lp_path != nullptr) {
    path = lp_path;
    free(lp_path);
  }
  return path;
}

bool ResizeMovePartition(const Partition::Ptr partition) {
  qDebug() << "ResizeMovePartition()" << partition;
  bool ok = false;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition = nullptr;
    if (partition->type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition->getSector());
    }
    if (lp_partition) {
      PedGeometry* geom = ped_geometry_new(lp_device, partition->start_sector,
                                           partition->getSectorLength());
      PedConstraint* constraint = nullptr;
      if (geom) {
        constraint = ped_constraint_exact(geom);
      }
      if (constraint) {
        ok = bool(ped_disk_set_partition_geom(lp_disk, lp_partition, constraint,
                                              partition->start_sector,
                                              partition->end_sector));
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

bool SetPartitionFlag(const Partition::Ptr partition,
                      PedPartitionFlag flag,
                      bool is_set) {
  qDebug() << "SetPartitionFlag()" << partition << flag << is_set;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  bool ok = false;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition =
        ped_disk_get_partition_by_sector(lp_disk, partition->getSector());
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

bool SetPartitionFlags(const Partition::Ptr partition) {
  for (PartitionFlag flag : partition->flags) {
    if (!SetPartitionFlag(partition,
                          static_cast<PedPartitionFlag>(flag),
                          true)) {
      return false;
    }
  }
  return true;
}

bool SetPartitionType(const Partition::Ptr partition) {
  qDebug() << "SetPartitionType:" << partition;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  bool ok = false;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    const QString fs_name = GetPedFsName(partition->fs);

    PedFileSystemType* fs_type = GetPedFsType(partition);
    PedPartition* lp_partition =
        ped_disk_get_partition_by_sector(lp_disk, partition->getSector());

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
                << partition;
  }

  return ok;
}

void SettleDevice(int timeout) {
  SpawnCmd("udevadm", {"settle", QString("--timeout=%1").arg(timeout)});
}

bool UpdatePartitionNumber(Partition::Ptr partition) {
  bool ok = false;
  PedDevice* lp_device = nullptr;
  PedDisk* lp_disk = nullptr;
  if (GetDeviceAndDisk(partition->device_path, lp_device, lp_disk)) {
    PedPartition* lp_partition = nullptr;
    if (partition->type == PartitionType::Extended) {
      lp_partition = ped_disk_extended_partition(lp_disk);
    } else {
      lp_partition = ped_disk_get_partition_by_sector(lp_disk,
                                                      partition->getSector());
    }
    if (lp_partition) {
      partition->partition_number = lp_partition->num;
      partition->path = GetPartitionPath(lp_partition);
      ok = true;
    } else {
      qCritical() << "UpdatePartitionNumber() lp_partition is nullptr";
    }

    DestroyDeviceAndDisk(lp_device, lp_disk);
  } else {
    qCritical() << "UpdatePartitionNumber() failed to get lp disk object"
                << partition;
  }

  return ok;
}

}  // namespace installer
