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

#include "ui/delegates/simple_partition_delegate.h"

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_util.h"

namespace installer {

SimplePartitionDelegate::SimplePartitionDelegate(QObject* parent)
    : QObject(parent),
      real_devices_(),
      virtual_devices_(),
      bootloader_path_(),
      operations_(),
      selected_partition_() {
  this->setObjectName("simple_partition_delegate");
}

bool SimplePartitionDelegate::canAddLogical(const Partition::Ptr partition) const {
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

bool SimplePartitionDelegate::canAddPrimary(const Partition::Ptr partition) const {
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

QStringList SimplePartitionDelegate::getOptDescriptions() const {
  QStringList descriptions;
  for (const Operation& operation : operations_) {
    descriptions.append(operation.description());
  }

  return descriptions;
}

bool SimplePartitionDelegate::isMBRPreferred() const {
  return IsMBRPreferred(real_devices_);
}

bool SimplePartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  return IsPartitionTableMatch(real_devices_, device_path);
}

void SimplePartitionDelegate::selectPartition(const Partition::Ptr partition) {
  selected_partition_ = partition;
}

bool SimplePartitionDelegate::setBootFlag() {
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
          if (operation.type == OperationType::Create || operation.type == OperationType::MountPoint) {
              if (operation.new_partition->mount_point == kMountPointBoot) {
                  operation.new_partition->flags.append(PartitionFlag::Boot);
                  found_boot = true;
              }
          }
      }
  }

  // At last, check / partition
  // Maybe origin partition is ext4, operation.type = Format
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

SimpleValidateState SimplePartitionDelegate::validate() const {
  // Policy:
  // * Returns ok if partition table of selected partition is empty.
  // * Check / partition is set.
  // * Check / partition is large enough.

  if (selected_partition_.isNull()) {
      return SimpleValidateState::RootMissing;
  }

  const Partition::Ptr root_partition = selected_partition_;

  // Check partition table is empty or not.
  const int device_index = DeviceIndex(virtual_devices_,
                                       root_partition->device_path);
  if (device_index == -1) {
    qCritical() << "validate() device index out of range:"
                << root_partition->device_path;
    return SimpleValidateState::RootMissing;
  }
  const Device::Ptr device = virtual_devices_.at(device_index);
  if (device->table == PartitionTableType::Empty) {
    return SimpleValidateState::Ok;
  }

  if (root_partition->device_path.isEmpty()) {
    return SimpleValidateState::RootMissing;
  }

   // If currently selected device reaches its maximum primary partitions and
  // root partition is a Freespace, it is impossible to created a new
  // primary partition->
  if ((root_partition->type == PartitionType::Unallocated) &&
      !this->canAddPrimary(root_partition) &&
      !this->canAddLogical(root_partition)) {
    return SimpleValidateState::MaxPrimPartErr;
  }

  const int root_required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const qint64 root_minimum_bytes = root_required * kGibiByte;
  const qint64 root_real_bytes = root_partition->getByteLength() + kMebiByte;
  if (root_real_bytes < root_minimum_bytes) {
    return SimpleValidateState::RootTooSmall;
  }

  return SimpleValidateState::Ok;
}

void SimplePartitionDelegate::resetOperations() {
  operations_.clear();

  virtual_devices_ = FilterInstallerDevice(real_devices_);
}

bool SimplePartitionDelegate::createPartition(const Partition::Ptr partition,
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
    new_device->partitions.clear();
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
                                  partition_type,
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
    qCritical() << "not supported partition type:" << partition_type;
    return false;
  }
}

bool SimplePartitionDelegate::createLogicalPartition(const Partition::Ptr partition,
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
    // TODO(xushaohua): Support extended partition in simple mode.
    qCritical() << "Cannot create extended partition in simple mode";
//    return false;

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

  const Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

bool SimplePartitionDelegate::createPrimaryPartition(const Partition::Ptr partition,
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

  int partition_number;
  // In simple mode, operations has never been applied to partition list.
  // So do it temporarily.
  Device::Ptr tmp_device (new Device(*device));
  for (const Operation& operation : operations_) {
    if ((operation.type == OperationType::NewPartTable &&
         operation.device->path == tmp_device->path) ||
        (operation.orig_partition->device_path == tmp_device->path)) {
      operation.applyToVisual(tmp_device);
    }
  }

  partition_number = AllocPrimaryPartitionNumber(tmp_device);
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

  Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

Partition::Ptr SimplePartitionDelegate::deletePartition(const Partition::Ptr partition) {
  Partition::Ptr new_partition (new Partition);
  new_partition->sector_size = partition->sector_size;
  new_partition->start_sector = partition->start_sector;
  new_partition->end_sector = partition->end_sector;
  new_partition->device_path = partition->device_path;
  new_partition->type = PartitionType::Unallocated;
  new_partition->freespace = new_partition->length;
  new_partition->fs = FsType::Empty;
  new_partition->status = PartitionStatus::Delete;

  // No need to merge operations.
  // No need to delete extended partition->

  Operation operation(OperationType::Delete, partition, new_partition);
  operations_.append(operation);

  return new_partition;
}

void SimplePartitionDelegate::formatPartition(const Partition::Ptr partition,
                                              FsType fs_type,
                                              const QString& mount_point) {
  qDebug() << "formatSimplePartition()" << partition << fs_type << mount_point;

  Partition::Ptr new_partition (new Partition);
  new_partition->sector_size = partition->sector_size;
  new_partition->start_sector = partition->start_sector;
  new_partition->end_sector = partition->end_sector;
  new_partition->path = partition->path;
  new_partition->device_path = partition->device_path;
  new_partition->fs = fs_type;
  new_partition->type = partition->type;
  new_partition->mount_point = mount_point;
  if (partition->status == PartitionStatus::Real) {
    new_partition->status = PartitionStatus::Format;
  } else if (partition->status == PartitionStatus::New ||
             partition->status == PartitionStatus::Format) {
    new_partition->status = partition->status;
  }

  Operation operation(OperationType::Format,partition, new_partition);
  operations_.append(operation);
}

bool SimplePartitionDelegate::formatWholeDevice(const QString& device_path,
                                                PartitionTableType type) {

  // Policy:
  //  * Create new partition table (msdos);
  //  * Create /boot with ext4, 500MiB;
  //  * Create swap partition with 4GiB;
  //  * Create / with ext4, remaining space (at most 300GiB);
  qDebug() << "formatWholeDevice()" << device_path;

  const int device_index = DeviceIndex(virtual_devices_, device_path);
  if (device_index == -1) {
    qCritical() << "formatWholeDevice() device index out of range:"
                << device_path;
    return false;
  }
  Device::Ptr device = virtual_devices_[device_index];
  qDebug() << "device selected to format:" << device;

  // Add NewPartTable operation.
  Device::Ptr new_device(new Device(*device));
  new_device->partitions.clear();
  new_device->table = type;
  const Operation operation(new_device);
  operations_.append(operation);
  // Update virtual device property at the same time.
  operation.applyToVisual(device);

  if (device->partitions.length() == 0) {
    qCritical() << "partition is empty" << device;
    return false;
  }

  Partition::Ptr unallocated = device->partitions.last();

  // Create /boot partition->
  const int boot_space = GetSettingsInt(kPartitionDefaultBootSpace);
  const qint64 boot_sectors = boot_space * kMebiByte / unallocated->sector_size;
  bool ok = createPrimaryPartition(unallocated,
                                   PartitionType::Normal,
                                   true,
                                   FsType::Ext4,
                                   kMountPointBoot,
                                   boot_sectors);
  if (!ok) {
    qCritical() << "Failed to create /boot partition on" << unallocated;
    return false;
  }
  Operation& boot_operation = operations_.last();
  boot_operation.applyToVisual(device);

  unallocated = device->partitions.last();
  // Create swap partition->
  const int swap_space = GetSettingsInt(kPartitionSwapPartitionSize);
  const qint64 swap_sectors = swap_space * kMebiByte / unallocated->sector_size;
  ok = createPrimaryPartition(unallocated,
                              PartitionType::Normal,
                              true,
                              FsType::LinuxSwap,
                              "",
                              swap_sectors);
  if (!ok) {
    qCritical() << "Failed to created swap partition:" << device;
    return false;
  }
  Operation& swap_operation = operations_.last();
  swap_operation.applyToVisual(device);

  const qint64 kRootMaximumSize = 300 * kGibiByte;
  const qint64 device_size = device->getByteLength();
  const qint64 root_size = (device_size > kRootMaximumSize) ?
                           kRootMaximumSize :
                           device_size;
  const qint64 root_sectors = root_size / device->sector_size;

  unallocated = device->partitions.last();
  ok = createPrimaryPartition(unallocated,
                              PartitionType::Normal,
                              true,
                              FsType::Ext4,
                              kMountPointRoot,
                              root_sectors);
  if (!ok) {
    qCritical() << "Failed to create / partition:" << device;
    return false;
  }

  qDebug() << "operations for simple disk mode:" << operations_;

  return true;
}


void SimplePartitionDelegate::onDeviceRefreshed(const DeviceList& devices) {
    real_devices_ = devices;
    operations_.clear();
    virtual_devices_ = FilterInstallerDevice(real_devices_);
    emit this->deviceRefreshed(virtual_devices_);
}

void SimplePartitionDelegate::onManualPartDone(const DeviceList& devices) {
  qDebug() << "SimplePartitionDelegate::onManualPartDone()" << devices;
  QString root_disk;
  QString root_path;
  QStringList mount_points;
  bool found_swap = false;
  QString esp_path;
  Device::Ptr root_device;

  // Check use-specified partitions with mount point.
  for (const Device::Ptr device : devices) {
    for (const Partition::Ptr partition : device->partitions) {
      if (!partition->mount_point.isEmpty()) {
        // Add in-used partitions to mount_point list.
        const QString record(QString("%1=%2").arg(partition->path)
                                 .arg( partition->mount_point));
        mount_points.append(record);
        if (partition->mount_point == kMountPointRoot) {
          root_disk = partition->device_path;
          root_path = partition->path;
          root_device = device;
        }
      }

      // Check linux-swap.
      if (partition->fs == FsType::LinuxSwap) {
        found_swap = true;

        // Add swap area to mount_point list.
        // NOTE(xushaohua): Multiple swap partitions may be set.
        const QString record(QString("%1=swap").arg(partition->path));
        mount_points.append(record);
      } else if (partition->fs == FsType::EFI && esp_path.isEmpty()) {
        // Only use the first EFI partition->
        esp_path = partition->path;
      }
    }
  }

  // Find esp partition on this device
  for (Partition::Ptr partition : root_device->partitions) {
      if (partition->fs == FsType::EFI && esp_path != partition->path) {
          esp_path = partition->path;
          break;
      }
  }

  if (!IsMBRPreferred(real_devices_)) {
    // Enable EFI mode. First check newly created EFI partition-> If not found,
    // check existing EFI partition->
    WriteUEFI(true);

    // There shall be only one EFI partition->
    if (esp_path.isEmpty()) {
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
  } else if (GetSettingsBool(kPartitionForceSwapFileInSimplePage)) {
    use_swap_file = true;
  } else {
    use_swap_file = IsSwapAreaNeeded();
  }
  WriteRequiringSwapFile(use_swap_file);
}

void SimplePartitionDelegate::setBootloaderPath(const QString& path) {
  bootloader_path_ = path;
}

}  // namespace installer
