// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/simple_partition_delegate.h"

#include "ui/delegates/partition_util.h"

namespace installer {

SimplePartitionDelegate::SimplePartitionDelegate(QObject* parent)
    : QObject(parent),
      real_devices_(),
      virtual_devices_(),
      bootloader_path_(),
      operations_() {
  this->setObjectName("simple_partition_delegate");
}

bool SimplePartitionDelegate::canAddLogical(const Partition& partition) const {
  const int index = DeviceIndex(virtual_devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = virtual_devices_.at(index);

  // Ignores gpt table.
  if (device.table != PartitionTableType::MsDos) {
    return false;
  }

  bool logical_ok = true;
  const int ext_index = ExtendedPartitionIndex(device.partitions);
  if (ext_index == -1) {
    // No extended partition found, so check a new primary partition is
    // available or not.
    if (GetPrimaryPartitions(device.partitions).length() >= device.max_prims) {
      logical_ok = false;
    }
  } else {
    // Check whether there is primary partition between |partition| and
    // extended partition.
    const Partition ext_partition = device.partitions.at(ext_index);
    const PartitionList prim_partitions = GetPrimaryPartitions(
        device.partitions);
    if (partition.end_sector < ext_partition.start_sector) {
      for (const Partition& prim_partition : prim_partitions) {
        if (prim_partition.end_sector > partition.start_sector &&
            prim_partition.start_sector < ext_partition.start_sector) {
          logical_ok = false;
        }
      }
    } else if (partition.start_sector > ext_partition.end_sector) {
      for (const Partition& prim_partition : prim_partitions) {
        if (prim_partition.end_sector < partition.start_sector &&
            prim_partition.start_sector > ext_partition.end_sector) {
          logical_ok =false;
        }
      }
    }
  }
  return logical_ok;
}

bool SimplePartitionDelegate::canAddPrimary(const Partition& partition) const {
  const int index = DeviceIndex(virtual_devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = virtual_devices_.at(index);
  const PartitionList prim_partitions = GetPrimaryPartitions(device.partitions);
  const PartitionList logical_partitions =
      GetLogicalPartitions(device.partitions);

  // Check primary type
  bool primary_ok = true;
  if (prim_partitions.length() >= device.max_prims) {
    primary_ok = false;
  } else {
    // Check whether |partition| is between two logical partitions.
    bool has_logical_before = false;
    bool has_logical_after = false;
    for (const Partition& logical_partition : logical_partitions) {
      if (logical_partition.start_sector < partition.start_sector) {
        has_logical_before = true;
      }
      if (logical_partition.end_sector > partition.end_sector) {
        has_logical_after = true;
      }
    }
    if (has_logical_after && has_logical_before) {
      primary_ok = false;
    }
  }

  return primary_ok;
}

bool SimplePartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  return IsPartitionTableMatch(real_devices_, device_path);
}

void SimplePartitionDelegate::resetOperations() {
  operations_.clear();

  virtual_devices_ = real_devices_;
}

bool SimplePartitionDelegate::createPartition(const Partition& partition,
                                              PartitionType partition_type,
                                              bool align_start,
                                              FsType fs_type,
                                              const QString& mount_point,
                                              qint64 total_sectors) {
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

bool SimplePartitionDelegate::createLogicalPartition(const Partition& partition,
                                                     bool align_start,
                                                     FsType fs_type,
                                                     const QString& mount_point,
                                                     qint64 total_sectors) {
  // Policy:
  // * Create extended partition if not found;
  // * If new logical partition is not contained in or is intersected with
  //   extended partition, enlarge extended partition.

  const int device_index = DeviceIndex(virtual_devices_, partition.device_path);
  if (device_index == -1) {
    qCritical() << "createLogicalPartition() device index out of range:"
                << partition.device_path;
    return false;
  }
  const Device& device = virtual_devices_.at(device_index);

  const int ext_index = ExtendedPartitionIndex(device.partitions);
  Partition ext_partition;
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
    ext_partition = device.partitions.at(ext_index);

    // Enlarge extended partition if needed.
    if (ext_partition.start_sector > partition.start_sector ||
        ext_partition.end_sector < partition.end_sector) {
      Partition new_ext_partition(ext_partition);
      new_ext_partition.start_sector = qMin(ext_partition.start_sector,
                                            partition.start_sector);
      new_ext_partition.end_sector = qMax(ext_partition.end_sector,
                                          partition.end_sector);

      AlignPartition(new_ext_partition);

      const Operation resize_ext_operation(OperationType::Resize,
                                           ext_partition,
                                           new_ext_partition);
      operations_.append(resize_ext_operation);

      ext_partition = new_ext_partition;
    }
  }

  Partition new_partition;
  new_partition.device_path = partition.device_path;
  new_partition.path = partition.path;
  new_partition.sector_size = partition.sector_size;
  new_partition.status = PartitionStatus::New;
  new_partition.type = PartitionType::Logical;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  const int partition_number = AllocLogicalPartitionNumber(device);
  if (partition_number < 0) {
    qCritical() << "Failed to allocate logical part number!";
    return false;
  }
  new_partition.changeNumber(partition_number);

  // space is required for the Extended Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition.
  const qint64 oneMebiByteSector = 1 * kMebiByte / partition.sector_size;
  if (align_start) {
    // Align from start of |partition|.
    // Add space for Extended Boot Record.
    const qint64 start_sector = qMax(partition.start_sector,
                                     ext_partition.start_sector);
    new_partition.start_sector = start_sector + oneMebiByteSector;

    const qint64 end_sector = qMin(partition.end_sector,
                                   ext_partition.end_sector);
    new_partition.end_sector = qMin(end_sector,
                                    total_sectors + new_partition.start_sector - 1);
  } else {
    new_partition.end_sector = qMin(partition.end_sector,
                                    ext_partition.end_sector);
    const qint64 start_sector = qMax(partition.start_sector,
                                     ext_partition.start_sector);
    new_partition.start_sector = qMax(start_sector + oneMebiByteSector,
                                      partition.end_sector - total_sectors + 1);
  }

  // Align to nearest MebiBytes.
  AlignPartition(new_partition);

  // Check partition sector range.
  // Also check whether partition size is less than 1MiB or not.
  if (new_partition.start_sector < partition.start_sector ||
      new_partition.start_sector >= partition.end_sector ||
      new_partition.getByteLength() < kMebiByte ||
      new_partition.end_sector > partition.end_sector) {
    qCritical() << "Invalid partition sector range";
    return false;
  }

  const Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

bool SimplePartitionDelegate::createPrimaryPartition(const Partition& partition,
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

  const int device_index = DeviceIndex(virtual_devices_, partition.device_path);
  if (device_index == -1) {
    qCritical() << "createPrimaryPartition() device index out of range:"
                << partition.device_path;
    return false;
  }
  Device& device = virtual_devices_[device_index];

  const qint64 oneMebiByteSector = 1 * kMebiByte / partition.sector_size;

  // Shrink extended partition if needed.
  const int ext_index = ExtendedPartitionIndex(device.partitions);
  if (partition_type == PartitionType::Normal && ext_index > -1) {
    const Partition ext_partition = device.partitions.at(ext_index);
    const PartitionList logical_parts = GetLogicalPartitions(device.partitions);
    if (logical_parts.isEmpty()) {
      // Remove extended partition if no logical partitions.
      Partition unallocated_partition;
      unallocated_partition.device_path = ext_partition.device_path;
      // Extended partition does not contain any sectors.
      // This new allocated partition will be merged to other unallocated
      // partitions.
      unallocated_partition.start_sector = ext_partition.start_sector;
      unallocated_partition.end_sector = ext_partition.end_sector;
      unallocated_partition.sector_size = ext_partition.sector_size;
      unallocated_partition.type = PartitionType::Unallocated;
      const Operation operation(OperationType::Delete,
                                ext_partition,
                                unallocated_partition);
      operations_.append(operation);

      // Remove extended partition from partition list explicitly.
      device.partitions.removeAt(ext_index);

    } else if (IsPartitionsJoint(ext_partition, partition)) {
      // Shrink extended partition to fit logical partitions.
      Partition new_ext_part(ext_partition);
      new_ext_part.start_sector = logical_parts.first().start_sector -
                                  oneMebiByteSector;
      new_ext_part.end_sector = logical_parts.last().end_sector;

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

  Partition new_partition;
  new_partition.device_path = partition.device_path;
  new_partition.path = partition.path;
  new_partition.sector_size = partition.sector_size;
  new_partition.status = PartitionStatus::New;
  new_partition.type = partition_type;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;

  int partition_number;
  // In simple mode, operations has never been applied to partition list.
  // So do it temporarily.
  Device tmp_device = device;
  for (const Operation& operation : operations_) {
    operation.applyToVisual(tmp_device.partitions);
  }

  partition_number = AllocPrimaryPartitionNumber(tmp_device);
  if (partition_number < 0) {
    qCritical() << "Failed to allocate primary partition number!";
    return false;
  }
  new_partition.changeNumber(partition_number);

  // Check whether space is required for the Master Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition.
  const bool need_mbr = (partition.start_sector <= oneMebiByteSector);
  if (align_start) {
    // Align from start of |partition|.
    if (need_mbr) {
      new_partition.start_sector = oneMebiByteSector;
    } else {
      new_partition.start_sector = partition.start_sector;
    }
    new_partition.end_sector = qMin(partition.end_sector,
                                    total_sectors + new_partition.start_sector - 1);
  } else {
    new_partition.end_sector = partition.end_sector;
    if (need_mbr) {
      new_partition.start_sector = qMax(oneMebiByteSector,
                                        partition.end_sector - total_sectors + 1);
    } else {
      new_partition.start_sector = qMax(partition.start_sector,
                                        partition.end_sector - total_sectors + 1);
    }
  }

  // Align to nearest MebiBytes.
  AlignPartition(new_partition);

  // Check partition sector range.
  // Also check whether partition size is less than 1MiB or not.
  if (new_partition.start_sector < partition.start_sector ||
      new_partition.start_sector >= partition.end_sector ||
      new_partition.getByteLength() < kMebiByte ||
      new_partition.end_sector > partition.end_sector) {
    qCritical() << "Invalid partition sector range"
                << ", new_partition:" << new_partition
                << ", partition:" << partition;
    return false;
  }

  Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  return true;
}

Partition SimplePartitionDelegate::deletePartition(const Partition& partition) {
  Partition new_partition;
  new_partition.sector_size = partition.sector_size;
  new_partition.start_sector = partition.start_sector;
  new_partition.end_sector = partition.end_sector;
  new_partition.device_path = partition.device_path;
  new_partition.type = PartitionType::Unallocated;
  new_partition.freespace = new_partition.length;
  new_partition.fs = FsType::Empty;
  new_partition.status = PartitionStatus::Delete;

  // No need to merge operations.
  // No need to delete extended partition.

  Operation operation(OperationType::Delete, partition, new_partition);
  operations_.append(operation);

  return new_partition;
}

void SimplePartitionDelegate::formatPartition(const Partition& partition,
                                              FsType fs_type,
                                              const QString& mount_point) {
  qDebug() << "formatSimplePartition()" << partition << fs_type << mount_point;

  Partition new_partition;
  new_partition.sector_size = partition.sector_size;
  new_partition.start_sector = partition.start_sector;
  new_partition.end_sector = partition.end_sector;
  new_partition.path = partition.path;
  new_partition.device_path = partition.device_path;
  new_partition.fs = fs_type;
  new_partition.type = partition.type;
  new_partition.mount_point = mount_point;
  if (partition.status == PartitionStatus::Real) {
    new_partition.status = PartitionStatus::Format;
  } else if (partition.status == PartitionStatus::New ||
             partition.status == PartitionStatus::Format) {
    new_partition.status = partition.status;
  }

  Operation operation(OperationType::Format,partition, new_partition);
  operations_.append(operation);
}

void SimplePartitionDelegate::onDeviceRefreshed(const DeviceList& devices) {
  real_devices_ = devices;
  // TODO(xushaohua): Update virtual device list.
}

void SimplePartitionDelegate::setBootloaderPath(const QString& path) {
  bootloader_path_ = path;
}

}  // namespace installer