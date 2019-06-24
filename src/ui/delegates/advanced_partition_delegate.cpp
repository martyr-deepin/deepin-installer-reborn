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

#include "ui/delegates/advanced_partition_delegate.h"

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_util.h"
#include "base/command.h"

namespace installer {

namespace {

// "unused" mount point.
const char kMountPointUnused[] = "unused";

const QMap<FsType, QString> FsFormatCmdMap{
    { FsType::Btrfs, QString("mkfs.btrfs") },
    { FsType::EFI, QString("mkfs.vfat") },
    { FsType::Ext2, QString("mkfs.ext2") },
    { FsType::Ext3, QString("mkfs.ext3") },
    { FsType::Ext4, QString("mkfs.ext4") },
    { FsType::F2fs, QString("mkfs.f2fs") },
    { FsType::Fat16, QString("mkfs.fat") },
    { FsType::Fat32, QString("mkfs.fat") },
    { FsType::Hfs, QString("/usr/bin/hformat") },
    { FsType::HfsPlus, QString("/usr/bin/hpfsck") },
    { FsType::Jfs, QString("mkfs.jfs") },
    { FsType::LinuxSwap, QString("mkswap") },
    { FsType::LVM2PV, QString("lvm") },
    { FsType::Nilfs2, QString("mkfs.nilfs2") },
    { FsType::NTFS, QString("mkfs.ntfs") },
    { FsType::Reiser4, QString("mkfs.reiser4") },
    { FsType::Reiserfs, QString("mkfs.reiserfs") },
    { FsType::Xfs, QString("mkfs.xfs") }
};

}  // namespace

AdvancedPartitionDelegate::AdvancedPartitionDelegate(QObject* parent)
    : QObject(parent),
      real_devices_(),
      virtual_devices_(),
      bootloader_path_(),
      operations_() {
  this->setObjectName("advanced_partition_delegate");
}

bool AdvancedPartitionDelegate::canAddLogical(
    const Partition::Ptr partition) const {
  const int index = DeviceIndex(virtual_devices_, partition->device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition->device_path;
    return false;
  }
  const Device::Ptr device = virtual_devices_.at(index);

  // If partition table is empty, always returns false.
  // Thus, at least one primary partition shall be created.
  if (device->table == PartitionTableType::Empty) {
    return false;
  }

  // Ignores gpt table.
  if (device->table != PartitionTableType::MsDos) {
    return false;
  }

  bool logical_ok = true;
  const int ext_index = ExtendedPartitionIndex(device->partitions);
  if (ext_index == -1) {
    // No extended partition found, so check a new primary partition is
    // available or not.
    if (GetPrimaryPartitions(device->partitions).length() >= device->max_prims) {
      logical_ok = false;
    }
  } else {
    // Check whether there is primary partition between |partition| and
    // extended partition->
    const Partition::Ptr ext_partition = device->partitions.at(ext_index);
    const PartitionList prim_partitions = GetPrimaryPartitions(
        device->partitions);
    if (partition->end_sector < ext_partition->start_sector) {
      for (const Partition::Ptr prim_partition : prim_partitions) {
        if (prim_partition->end_sector > partition->start_sector &&
            prim_partition->start_sector < ext_partition->start_sector) {
          logical_ok = false;
        }
      }
    } else if (partition->start_sector > ext_partition->end_sector) {
      for (const Partition::Ptr prim_partition : prim_partitions) {
        if (prim_partition->end_sector < partition->start_sector &&
            prim_partition->start_sector > ext_partition->end_sector) {
          logical_ok =false;
        }
      }
    }
  }
  return logical_ok;
}

bool AdvancedPartitionDelegate::canAddPrimary(
    const Partition::Ptr partition) const {
  const int index = DeviceIndex(virtual_devices_, partition->device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition->device_path;
    return false;
  }
  const Device::Ptr device = virtual_devices_.at(index);

  // If partition table is empty, always returns true.
  if (device->table == PartitionTableType::Empty) {
    return true;
  }

  const PartitionList prim_partitions = GetPrimaryPartitions(device->partitions);
  const PartitionList logical_partitions =
      GetLogicalPartitions(device->partitions);

  // Check primary type
  bool primary_ok = true;
  if (prim_partitions.length() >= device->max_prims) {
    primary_ok = false;
  } else {
    // Check whether |partition| is between two logical partitions.
    bool has_logical_before = false;
    bool has_logical_after = false;
    for (const Partition::Ptr logical_partition : logical_partitions) {
      if (logical_partition->start_sector < partition->start_sector) {
        has_logical_before = true;
      }
      if (logical_partition->end_sector > partition->end_sector) {
        has_logical_after = true;
      }
    }
    if (has_logical_after && has_logical_before) {
      primary_ok = false;
    }
  }

  return primary_ok;
}

FsTypeList AdvancedPartitionDelegate::getFsTypeList() const {
  FsTypeList fs_types;
  if (fs_types.isEmpty()) {
    const QString name = GetSettingsString(kPartitionSupportedFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      // check fs can be used
      const QString cmd = FsFormatCmdMap[GetFsTypeByName(fs_name)];
      if (cmd.isEmpty()) continue;

      if (!SpawnCmd("which", QStringList() << cmd)) continue;

      FsType type = GetFsTypeByName(fs_name);
      fs_types.append(type);
    }
  }
  return fs_types;
}

FsTypeList AdvancedPartitionDelegate::getBootFsTypeList() const {
  FsTypeList fs_types;
  if (fs_types.isEmpty()) {
    const QString name = GetSettingsString(kPartitionBootPartitionFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      FsType type = GetFsTypeByName(fs_name);
      fs_types.append(type);
    }
  }
  return fs_types;}

QStringList AdvancedPartitionDelegate::getMountPoints() const {
  QStringList mount_points;
  if (mount_points.isEmpty()) {
    // Read available mount points.
    mount_points = GetSettingsStringList(kPartitionMountPoints);

    // Replace "unused" mount point with ""
    for (QString& mount_point : mount_points) {
      if (mount_point == kMountPointUnused) {
        mount_point = "";
      }
    }
  }
  return mount_points;
}

QStringList AdvancedPartitionDelegate::getOptDescriptions() const {
  QStringList descriptions;
  for (const Operation& operation : operations_) {
    descriptions.append(operation.description());
  }

  return descriptions;
}

Partition::Ptr AdvancedPartitionDelegate::getRealPartition(
    const Partition::Ptr virtual_partition) const {
  const int index = DeviceIndex(real_devices_, virtual_partition->device_path);
  if (index == -1) {
    qWarning() << "failed to find device:" << virtual_partition->device_path;
    return Partition::Ptr();
  }

  for (const Partition::Ptr partition : real_devices_.at(index)->partitions) {
    // Ignores extended partition->
    if (partition->type == PartitionType::Extended) {
      continue;
    }
    if ((partition->start_sector <= virtual_partition->start_sector) &&
        (partition->end_sector >= virtual_partition->end_sector)) {
      return partition;
    }
  }

  qWarning() << "Failed to find partition at:" << virtual_partition;
  return Partition::Ptr();
}

QList<Device::Ptr> AdvancedPartitionDelegate::getAllUsedDevice() const
{
    QList<Device::Ptr> list;

    for (const Operation& operation : operations_) {
        if (operation.type != OperationType::NewPartTable) {
            for (const Device::Ptr device : real_devices_) {
                if (device->path == operation.orig_partition->device_path) {
                    list << device;
                    break;
                }
            }
        }
    }

    return list;
}

bool AdvancedPartitionDelegate::isMBRPreferred() const {
  return IsMBRPreferred(real_devices_);
}

bool AdvancedPartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  return IsPartitionTableMatch(real_devices_, device_path);
}

bool AdvancedPartitionDelegate::setBootFlag() {
  bool found_boot = false;

  // First check new EFI partition
  for (Operation& operation : operations_) {
      if (operation.type == OperationType::Create || operation.type == OperationType::MountPoint) {
          if (operation.new_partition->fs == FsType::EFI) {
              operation.new_partition->flags.append(PartitionFlag::Boot);
              operation.new_partition->flags.append(PartitionFlag::ESP);
              found_boot = true;
          }
      }
  }

  // Check existing EFI partition
  for (const Device::Ptr device : virtual_devices_) {
    for (const Partition::Ptr partition : device->partitions) {
      if (partition->fs == FsType::EFI) {
        return true;
      }
    }
  }

  // Check /boot partition
  if (!found_boot) {
      for (Operation& operation : operations_) {
          if (operation.type == OperationType::Create ||
              operation.type == OperationType::MountPoint ||
              operation.type == OperationType::Format) {
              if (operation.new_partition->mount_point == kMountPointBoot) {
                  operation.new_partition->flags.append(PartitionFlag::Boot);
                  found_boot = true;
              }
          }
      }
  }

  // At last, check / partition
  if (!found_boot) {
      for (Operation& operation : operations_) {
          if (operation.type == OperationType::Create ||
              operation.type == OperationType::MountPoint ||
              operation.type == OperationType::Format) {
              if (operation.new_partition->mount_point == kMountPointRoot) {
                  operation.new_partition->flags.append(PartitionFlag::Boot);
                  found_boot = true;
              }
          }
      }
  }
  return found_boot;
}

AdvancedValidateStates AdvancedPartitionDelegate::validate() const {
  AdvancedValidateStates states;
  bool found_efi = false;
  bool efi_large_enough = false;
  bool found_root = false;
  bool root_large_enough = false;
  bool found_boot = false;
  bool boot_large_enough = false;

  // Filesystem of /boot and /.
  FsType boot_fs = FsType::Empty;
  FsType root_fs = FsType::Empty;

  // Partition number of /boot and /.
  int boot_part_number = -1;
  int root_part_number = -1;

  const int root_required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const int boot_recommended = GetSettingsInt(kPartitionDefaultBootSpace);
  const int efi_recommended = GetSettingsInt(kPartitionDefaultEFISpace);
  const int efi_minimum = GetSettingsInt(kPartitionEFIMinimumSpace);

  for (const Device::Ptr device : virtual_devices_) {
    for (const Partition::Ptr partition : device->partitions) {
      if (partition->mount_point == kMountPointRoot) {
        // Check / partition->
        found_root = true;
        root_fs = partition->fs;
        root_part_number = partition->partition_number;
        const qint64 root_real_bytes = partition->getByteLength() + kMebiByte;
        const qint64 root_minimum_bytes = root_required * kGibiByte;
        root_large_enough = (root_real_bytes >= root_minimum_bytes);

      } else if (partition->mount_point == kMountPointBoot) {
        // Check /boot partition->
        found_boot = true;
        boot_fs = partition->fs;
        root_part_number = partition->partition_number;
        const qint64 boot_recommend_bytes = boot_recommended * kMebiByte;
        // Add 1Mib to partition size.
        const qint64 boot_real_bytes = partition->getByteLength() + kMebiByte;
        boot_large_enough = (boot_real_bytes >= boot_recommend_bytes);

      } else if (partition->fs == FsType::EFI) {
        // Check EFI partition->
        found_efi = true;

        if (partition->status == PartitionStatus::Real) {
          // For existing EFI partition->
          const qint64 efi_minimum_bytes = efi_minimum * kMebiByte;
          const qint64 efi_real_bytes = partition->getByteLength() + kMebiByte;
          efi_large_enough = (efi_real_bytes >= efi_minimum_bytes);
        } else {
          // For newly created EFI partition->
          const qint64 efi_recommended_bytes = efi_recommended * kMebiByte;
          const qint64 efi_real_bytes = partition->getByteLength() + kMebiByte;
          efi_large_enough = (efi_real_bytes >= efi_recommended_bytes);
        }
      }
    }
  }

  if (found_root) {
    // Check root size only if root is set.
    if (!root_large_enough) {
      states.append(AdvancedValidateState::RootTooSmall);
    }
  } else {
    states.append(AdvancedValidateState::RootMissing);
  }

  // Check whether efi filesystem exists.
  if (!this->isMBRPreferred()) {
    if (found_efi) {
      if (!efi_large_enough) {
        states.append(AdvancedValidateState::EfiTooSmall);
      }
    } else {
      states.append(AdvancedValidateState::EfiMissing);
    }
  }

  if (found_boot && !boot_large_enough) {
    states.append(AdvancedValidateState::BootTooSmall);
  }

  // Check filesystem type is suitable for /boot folder.
  if (found_boot || found_root) {
    const FsType boot_root_fs = found_boot ? boot_fs : root_fs;
    const FsTypeList boot_fs_list = this->getBootFsTypeList();
    if (!boot_fs_list.contains(boot_root_fs)) {
      states.append(AdvancedValidateState::BootFsInvalid);
    }
  }

  // If /boot folder is required to be the first partition, validate it.
  if (GetSettingsBool(kPartitionBootOnFirstPartition)) {
    const int boot_root_part_num = found_boot ?
                                   boot_part_number :
                                   root_part_number;
    // If /boot or / is set, validate its partition number.
    if ((boot_root_part_num != -1) && (boot_root_part_num != 1)) {
      states.append(AdvancedValidateState::BootPartNumberInvalid);
    }
  }

  return states;
}

bool AdvancedPartitionDelegate::createPartition(const Partition::Ptr partition,
                                                PartitionType partition_type,
                                                bool align_start,
                                                FsType fs_type,
                                                const QString& mount_point,
                                                qint64 total_sectors) {
  // Policy:
  // * If partition table is empty, create a new one.
  const int device_index = DeviceIndex(virtual_devices_, partition->device_path);
  if (device_index == -1) {
    qCritical() << "createPartition() device index out of range:"
                << partition->device_path;
    return false;
  }
  Device::Ptr device = virtual_devices_[device_index];

  if (device->table == PartitionTableType::Empty) {
    // Add NewPartTable operation.
    Device::Ptr new_device(new Device(*device));
    new_device->table = IsEfiEnabled() ?
                       PartitionTableType::GPT :
                       PartitionTableType::MsDos;
    //NOTE: GPT table need 33 sectors in the end.
    if (new_device->table == PartitionTableType::GPT) {
        partition->length -= 33;
        partition->end_sector -= 33;
    }
    const Operation operation(new_device);
    operations_.append(operation);
    // Update virtual device property at the same time.
    operation.applyToVisual(device);
  }

  if (partition_type == PartitionType::Normal) {
    return createPrimaryPartition(partition,
                                  PartitionType::Normal,
                                  align_start,
                                  fs_type,
                                  mount_point,
                                  total_sectors);
  } else if (partition_type == PartitionType::Logical) {
    return createLogicalPartition(partition,
                                  align_start,
                                  fs_type,
                                  mount_point,
                                  total_sectors);
  } else {
    qCritical() << "createPartition() not supported type:" << partition_type;
    return false;
  }
}

bool
AdvancedPartitionDelegate::createLogicalPartition(const Partition::Ptr partition,
                                                  bool align_start,
                                                  FsType fs_type,
                                                  const QString& mount_point,
                                                  qint64 total_sectors) {
// Policy:
  // * Create extended partition if not found;
  // * If new logical partition is not contained in or is intersected with
  //   extended partition, enlarge extended partition->

  const int device_index = DeviceIndex(virtual_devices_, partition->device_path);
  if (device_index == -1) {
    qCritical() << "createLogicalPartition() device index out of range:"
                << partition->device_path;
    return false;
  }
  const Device::Ptr device = virtual_devices_.at(device_index);

  const int ext_index = ExtendedPartitionIndex(device->partitions);
  Partition::Ptr ext_partition (new Partition);
  if (ext_index == -1) {
    // No extended partition found, create one.
    if (!createPrimaryPartition(partition,
                                PartitionType::Extended,
                                align_start,
                                FsType::Empty,
                                "",
                                total_sectors)) {
      qCritical() << "Failed to create extended partition";
      return false;
    }

    ext_partition = operations_.last().new_partition;
  } else {
    // No need to add extended partition or enlarge it.
    ext_partition = device->partitions.at(ext_index);

    // Enlarge extended partition if needed.
    if (ext_partition->start_sector > partition->start_sector ||
        ext_partition->end_sector < partition->end_sector) {
      Partition::Ptr new_ext_partition(new Partition(*ext_partition));
      new_ext_partition->start_sector = qMin(ext_partition->start_sector,
                                            partition->start_sector);
      new_ext_partition->end_sector = qMax(ext_partition->end_sector,
                                          partition->end_sector);

      AlignPartition(new_ext_partition);

      const Operation resize_ext_operation(OperationType::Resize,
                                           ext_partition,
                                           new_ext_partition);
      operations_.append(resize_ext_operation);

      ext_partition = new_ext_partition;
    }
  }

  Partition::Ptr new_partition (new Partition);
  new_partition->device_path = partition->device_path;
  new_partition->path = partition->path;
  new_partition->sector_size = partition->sector_size;
  new_partition->status = PartitionStatus::New;
  new_partition->type = PartitionType::Logical;
  new_partition->fs = fs_type;
  new_partition->mount_point = mount_point;
  const int partition_number = AllocLogicalPartitionNumber(device);
  if (partition_number < 0) {
    qCritical() << "Failed to allocate logical part number!";
    return false;
  }
  new_partition->changeNumber(partition_number);

  // space is required for the Extended Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition->
  const qint64 oneMebiByteSector = 1 * kMebiByte / partition->sector_size;
  if (align_start) {
    // Align from start of |partition|.
    // Add space for Extended Boot Record.
    const qint64 start_sector = qMax(partition->start_sector,
                                     ext_partition->start_sector);
    new_partition->start_sector = start_sector + oneMebiByteSector;

    const qint64 end_sector = qMin(partition->end_sector,
                                   ext_partition->end_sector);
    new_partition->end_sector = qMin(end_sector,
                                    total_sectors + new_partition->start_sector - 1);
  } else {
    new_partition->end_sector = qMin(partition->end_sector,
                                    ext_partition->end_sector);
    const qint64 start_sector = qMax(partition->start_sector,
                                     ext_partition->start_sector);
    new_partition->start_sector = qMax(start_sector + oneMebiByteSector,
                                      partition->end_sector - total_sectors + 1);
  }

  // Align to nearest MebiBytes.
  AlignPartition(new_partition);

  // Check partition sector range.
  // Also check whether partition size is less than 1MiB or not.
  if (new_partition->start_sector < partition->start_sector ||
      new_partition->start_sector >= partition->end_sector ||
      new_partition->getByteLength() < kMebiByte ||
      new_partition->end_sector > partition->end_sector) {
    qCritical() << "Invalid partition sector range";
    return false;
  }

  // Reset mount-point before append operation to advanced operation list.
  this->resetOperationMountPoint(mount_point);
  const Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

bool
AdvancedPartitionDelegate::createPrimaryPartition(const Partition::Ptr partition,
                                                  PartitionType partition_type,
                                                  bool align_start,
                                                  FsType fs_type,
                                                  const QString& mount_point,
                                                  qint64 total_sectors) {
// Policy:
  // * If new primary partition is contained in or intersected with
  //   extended partition, shrink extended partition or delete it if no other
  //   logical partitions.

  if (partition_type != PartitionType::Normal &&
      partition_type != PartitionType::Extended) {
    qCritical() << "createPrimaryPartition() invalid part type:"
                << partition_type;
    return false;
  }

  const int device_index = DeviceIndex(virtual_devices_, partition->device_path);
  if (device_index == -1) {
    qCritical() << "createPrimaryPartition() device index out of range:"
                << partition->device_path;
    return false;
  }
  Device::Ptr device = virtual_devices_[device_index];

  const qint64 oneMebiByteSector = 1 * kMebiByte / partition->sector_size;

  // Shrink extended partition if needed.
  const int ext_index = ExtendedPartitionIndex(device->partitions);
  if (partition_type == PartitionType::Normal && ext_index > -1) {
    const Partition::Ptr ext_partition = device->partitions.at(ext_index);
    const PartitionList logical_parts = GetLogicalPartitions(device->partitions);
    if (logical_parts.isEmpty()) {
      // Remove extended partition if no logical partitions.
      Partition::Ptr unallocated_partition (new Partition);
      unallocated_partition->device_path = ext_partition->device_path;
      // Extended partition does not contain any sectors.
      // This new allocated partition will be merged to other unallocated
      // partitions.
      unallocated_partition->start_sector = ext_partition->start_sector;
      unallocated_partition->end_sector = ext_partition->end_sector;
      unallocated_partition->sector_size = ext_partition->sector_size;
      unallocated_partition->type = PartitionType::Unallocated;
      const Operation operation(OperationType::Delete,
                                ext_partition,
                                unallocated_partition);
      operations_.append(operation);

      // Remove extended partition from partition list explicitly.
      device->partitions.removeAt(ext_index);

    } else if (IsPartitionsJoint(ext_partition, partition)) {
      // Shrink extended partition to fit logical partitions.
      Partition::Ptr new_ext_part(new Partition(*ext_partition));
      new_ext_part->start_sector = logical_parts.first()->start_sector -
                                  oneMebiByteSector;
      new_ext_part->end_sector = logical_parts.last()->end_sector;

      if (IsPartitionsJoint(new_ext_part, partition)) {
        qCritical() << "Failed to shrink extended partition!";
        return false;
      }

      const Operation operation(OperationType::Resize,
                                ext_partition,
                                new_ext_part);
      operations_.append(operation);
    }
  }

  Partition::Ptr new_partition (new Partition);
  new_partition->device_path = partition->device_path;
  new_partition->path = partition->path;
  new_partition->sector_size = partition->sector_size;
  new_partition->status = PartitionStatus::New;
  new_partition->type = partition_type;
  new_partition->fs = fs_type;
  new_partition->mount_point = mount_point;

  const int partition_number = AllocPrimaryPartitionNumber(device);
  if (partition_number < 0) {
    qCritical() << "Failed to allocate primary partition number!";
    return false;
  }
  new_partition->changeNumber(partition_number);

  // Check whether space is required for the Master Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition->
  const bool need_mbr = (partition->start_sector <= oneMebiByteSector);
  if (align_start) {
    // Align from start of |partition|.
    if (need_mbr) {
      new_partition->start_sector = oneMebiByteSector;
    } else {
      new_partition->start_sector = partition->start_sector;
    }
    new_partition->end_sector = qMin(partition->end_sector,
                                    total_sectors + new_partition->start_sector - 1);
  } else {
    new_partition->end_sector = partition->end_sector;
    if (need_mbr) {
      new_partition->start_sector = qMax(oneMebiByteSector,
                                        partition->end_sector - total_sectors + 1);
    } else {
      new_partition->start_sector = qMax(partition->start_sector,
                                        partition->end_sector - total_sectors + 1);
    }
  }

  // Align to nearest MebiBytes.
  AlignPartition(new_partition);

  // Check partition sector range.
  // Also check whether partition size is less than 1MiB or not.
  if (new_partition->start_sector < partition->start_sector ||
      new_partition->start_sector >= partition->end_sector ||
      new_partition->getByteLength() < kMebiByte ||
      new_partition->end_sector > partition->end_sector) {
    qCritical() << "Invalid partition sector range"
                << ", new_partition:" << new_partition
                << ", partition:" << partition;
    return false;
  }

  // Reset mount-point before appending operation to advanced operation list.
  this->resetOperationMountPoint(mount_point);
  Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

void AdvancedPartitionDelegate::deletePartition(const Partition::Ptr partition) {
  // Policy:
  //  * Remove selected partition->
  //  * Merge unallocated partitions.
  //  * Remove extended partition if no logical partitions found.
  //  * Update partition number if needed.

  Partition::Ptr new_partition(new Partition(*partition));
  new_partition->partition_number = -1;
  new_partition->device_path  = partition->device_path;
  new_partition->sector_size  = partition->sector_size;
  new_partition->start_sector = partition->start_sector;
  new_partition->end_sector   = partition->end_sector;
  new_partition->type         = PartitionType::Unallocated;
  new_partition->fs           = FsType::Empty;
  new_partition->status       = PartitionStatus::Delete;

  if (partition->status == PartitionStatus::New) {
    // If status of old partition is New, there shall be a CreateOperation
    // which generates that partition-> Merge that CreateOperation
    // with DeleteOperation.

    // TODO(xushaohua): Move to operation.h
    for (int index = operations_.length() - 1; index >= 0; --index) {
        const Operation& operation = operations_.at(index);
        if (operation.type == OperationType::Create &&
            *operation.new_partition.data() == *partition.data()) {
            partition->type   = PartitionType::Unallocated;
            partition->fs     = FsType::Empty;
            partition->status = PartitionStatus::Delete;

            qDebug() << "delete partition info: " << *partition.data();

            const qint64 start_size = operation.orig_partition->start_sector;

            operations_.removeAt(index);

            // 修改操作，把相邻分区的operation中的orig partition向前补齐
            const qint64 end_size = partition->end_sector + 1;
            for (auto it = operations_.begin(); it != operations_.end(); ++it) {
                if (it->type == OperationType::Create &&
                    partition->device_path == it->orig_partition->device_path &&
                    it->orig_partition->start_sector == end_size) {
                    it->orig_partition->start_sector = start_size;
                }
            }
            break;
        }
    }
  } else {
      Operation operation(OperationType::Delete, partition, new_partition);
      operations_.append(operation);
      qDebug() << "add delete operation" << *new_partition.data();
  }

  if (partition->type == PartitionType::Logical) {
    // Delete extended partition if needed.
    const int device_index = DeviceIndex(virtual_devices_,
                                         partition->device_path);
    if (device_index == -1) {
      qCritical() << "deletePartition() Failed to get device:"
                  << partition->device_path;
      return;
    }

    Device::Ptr device = virtual_devices_[device_index];
    PartitionList& partitions = device->partitions;

    const int ext_index = ExtendedPartitionIndex(partitions);
    if (ext_index > -1) {
      const PartitionList logical_parts = GetLogicalPartitions(partitions);

      // Only one logical partition, and it has been removed.
      // Or logical partition list is empty.
      if ((logical_parts.length() == 1 && logical_parts.at(0) == partition) ||
          (logical_parts.length() == 0)) {
        const Partition::Ptr ext_partition = partitions.at(ext_index);
        Partition::Ptr unallocated_partition (new Partition);
        unallocated_partition->device_path = ext_partition->device_path;
        // Extended partition does not contain any sectors.
        // This new allocated partition will be merged to other unallocated
        // partitions.
        unallocated_partition->start_sector = ext_partition->start_sector;
        unallocated_partition->end_sector = ext_partition->end_sector;
        unallocated_partition->sector_size = ext_partition->sector_size;
        unallocated_partition->type = PartitionType::Unallocated;
        const Operation operation(OperationType::Delete,
                                  ext_partition,
                                  unallocated_partition);
        operations_.append(operation);
      }
    }
  }

  // TODO(xushaohua): Update partition number.
}

void AdvancedPartitionDelegate::formatPartition(const Partition::Ptr partition,
                                                FsType fs_type,
                                                const QString& mount_point) {
  qDebug() << "formatPartition()" << partition << mount_point;

  this->resetOperationMountPoint(mount_point);

  // Update partition of old operation, instead of adding a new one.
  // TODO(xushaohua): Move to operation.h
  if (partition->status == PartitionStatus::New ||
      partition->status == PartitionStatus::Format) {
    for (int index = operations_.length() - 1; index >= 0; --index) {
      Operation& operation = operations_[index];
      if ((operation.new_partition->path == partition->path) &&
          (operation.type == OperationType::Format ||
           operation.type == OperationType::Create)) {
        operation.new_partition->mount_point = mount_point;
        operation.new_partition->fs = fs_type;
        return;
      }
    }
  }

  Partition::Ptr new_partition (new Partition);
  new_partition->sector_size = partition->sector_size;
  new_partition->start_sector = partition->start_sector;
  new_partition->end_sector = partition->end_sector;
  new_partition->path = partition->path;
  new_partition->device_path = partition->device_path;
  new_partition->fs = fs_type;
  new_partition->type = partition->type;
  new_partition->mount_point = mount_point;
  new_partition->status = PartitionStatus::Format;

  Operation operation(OperationType::Format, partition, new_partition);
  operations_.append(operation);
}

void AdvancedPartitionDelegate::onDeviceRefreshed(const DeviceList& devices) {
  qDebug() << "device refreshed():" << devices;
  real_devices_ = devices;
  operations_.clear();
  virtual_devices_ = FilterInstallerDevice(real_devices_);

  emit this->deviceRefreshed(virtual_devices_);
}

void AdvancedPartitionDelegate::onManualPartDone(const DeviceList& devices) {
  qDebug() << "AdvancedPartitionDelegate::onManualPartDone()" << devices;

    QString        root_disk;
    QString        root_path;
    QStringList    mount_points;
    bool           found_swap = false;
    QString        esp_path;
    Device::Ptr    root_device;
    Partition::Ptr efi_partition;

    // Check use-specified partitions with mount point.
    for (const Device::Ptr device : devices) {
        for (const Partition::Ptr partition : device->partitions) {
            if (!partition->mount_point.isEmpty()) {
                // Add used partitions to mount_point list.
                const QString record(QString("%1=%2").arg(partition->path).arg(partition->mount_point));
                mount_points.append(record);
                if (partition->mount_point == kMountPointRoot) {
                    root_disk   = partition->device_path;
                    root_path   = partition->path;
                    root_device = device;
                }
            }

            if (partition->fs == FsType::LinuxSwap) {
                found_swap = true;

                // Add swap area to mount_point list.
                // NOTE(xushaohua): Multiple swap partitions may be set.
                const QString record(QString("%1=swap").arg(partition->path));
                mount_points.append(record);
            }
            else if (partition->fs == FsType::EFI && esp_path.isEmpty()) {
                // NOTE(xushaohua): There shall be only one EFI partition->
                esp_path = partition->path;
            }
        }
    }

    // Find this device efi partition
    for (Partition::Ptr partition : root_device->partitions) {
        if (partition->fs == FsType::EFI && esp_path != partition->path) {
            // NOTE(lxz): maybe we shoud check efi freespcae
            esp_path = partition->path;
            break;
        }
    }

  if (!IsMBRPreferred(real_devices_)) {
    // Enable EFI mode. First check newly created EFI partition-> If not found,
    // check existing EFI partition->
    WriteUEFI(true);

    if (esp_path.isEmpty()) {
      // We shall never reach here.
      qCritical() << "esp path is empty!";
    }
    WritePartitionInfo(root_disk, root_path, esp_path, mount_points.join(';'));
  } else {
    WriteUEFI(false);
    // In legacy mode.
    WritePartitionInfo(root_disk, root_path, bootloader_path_,
                       mount_points.join(';'));
  }

  // Create swap file if physical memory is less than 4Gib and
  // swap partition is not found.
  bool use_swap_file;
  if (found_swap) {
    use_swap_file = false;
  } else if (!GetSettingsBool(kPartitionEnableSwapFile)) {
    use_swap_file = false;
  } else if (!GetSettingsBool(kPartitionEnableSwapFileInAdvancedPage)) {
    use_swap_file = false;
  } else {
    use_swap_file = IsSwapAreaNeeded();
  }
  WriteRequiringSwapFile(use_swap_file);
}

void AdvancedPartitionDelegate::refreshVisual() {
// Filters partition list based on the following policy:
  // * Remove extended partition if no logical partition exists;
  // * Merge unallocated partition with next unallocated one;
  // * Ignore partitions with size less than 100Mib;

  virtual_devices_ = FilterInstallerDevice(real_devices_);

  for (Device::Ptr device : virtual_devices_) {
      device->partitions = FilterFragmentationPartition(device->partitions);
  }

  for (Device::Ptr device : virtual_devices_) {
      // Merge unallocated partitions.
      MergeUnallocatedPartitions(device->partitions);

      for (Operation& operation : operations_) {
          if ((operation.type == OperationType::NewPartTable && *operation.device.data() == *device.data()) ||
              (operation.type != OperationType::NewPartTable && operation.orig_partition->device_path == device->path)) {
              operation.applyToVisual(device);
          }
      }

      // Merge unallocated partitions.
      MergeUnallocatedPartitions(device->partitions);
  }

  qDebug() << "devices:" << virtual_devices_;
  qDebug() << "operations:" << operations_;
  emit this->deviceRefreshed(virtual_devices_);
}

void AdvancedPartitionDelegate::resetOperationMountPoint(
    const QString& mount_point) {
  qDebug() << "resetOperationMountPoint:" << mount_point;
  for (int index = operations_.length() - 1; index >= 0; --index) {
    Operation& operation = operations_[index];
    if (operation.type == OperationType::NewPartTable) continue; //skip create new part table

    if (operation.new_partition->mount_point == mount_point) {
      if (operation.type == OperationType::MountPoint) {
        // TODO(xushaohua): move to operation.h
        // Remove MountPointOperation with same mount point.
        operations_.removeAt(index);
        return;
      } else {
        // Clear mount point of old operation.
        operation.new_partition->mount_point = "";
        qDebug() << "Clear mount-point of operation:" << operation;
        return;
      }
    }
  }
}

void AdvancedPartitionDelegate::setBootloaderPath(const QString& path) {
  bootloader_path_ = path;
}

bool AdvancedPartitionDelegate::unFormatPartition(const Partition::Ptr partition) {
  Q_ASSERT(partition->status == PartitionStatus::Format);
  if (partition->status == PartitionStatus::Format) {
    for (int index = operations_.length() - 1; index >= 0; --index) {
      const Operation& operation = operations_.at(index);
      // Remove the last FormatOperation if its new_partition range is the
      // same with |partition|.
      if (operation.type == OperationType::Format &&
          operation.new_partition == partition) {
        operations_.removeAt(index);
        return true;
      }
    }
    qCritical() << "No appropriate FormatPartition found:" << partition;
  } else {
    qCritical() << "Invalid partition status:" << partition;
  }
  return false;
}

void AdvancedPartitionDelegate::updateMountPoint(const Partition::Ptr partition,
                                                 const QString& mount_point) {
  qDebug() << "PartitionDelegate::updateMountPoint()" << partition->path
           << mount_point;

  // Reset mount-point of operation with the same mount-point.
  this->resetOperationMountPoint(mount_point);

  if (!mount_point.isEmpty()) {
    // Append MountPointOperation only if |mount_point| is not empty.
    Partition::Ptr new_partition(new Partition(*partition));
    new_partition->mount_point = mount_point;
    // No need to update partition status.
    Operation operation(OperationType::MountPoint, partition, new_partition);
    operations_.append(operation);
  }
}

}  // namespace installer
