// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_delegate.h"

#include <QDebug>
#include <QThread>

#include "base/thread_util.h"
#include "partman/partition_manager.h"
#include "partman/utils.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"

namespace installer {

namespace {

const char kMountPointUnused[] = "unused";

// If partition size is less than this value, hide it from partition list.
const qint64 kMinimumPartitionSizeToDisplay = 1 * kMebiByte;

// Calculate new primary partition number.
// Returns -1 on failed.
int AllocPrimaryPartitionNumber(const Device& device) {
  for (int num = 1; num <= device.max_prims; num++) {
    bool in_use = false;
    for (const Partition& partition : device.partitions) {
      if (partition.partition_number == num) {
        in_use = true;
        break;
      }
    }

    if (!in_use) {
      return num;
    }
  }
  return -1;
}

// Calculate new logical partition number.
int AllocLogicalPartitionNumber(const Device& device) {
  int num = device.max_prims;
  for (const Partition& partition : device.partitions) {
    if (partition.partition_number >= num) {
      num = partition.partition_number;
    }
  }
  return num + 1;
}

}  // namespace

PartitionDelegate::PartitionDelegate(QObject* parent)
    : QObject(parent),
      partition_manager_(new PartitionManager()),
      partition_thread_(new QThread(this)),
      devices_(),
      real_devices_(),
      simple_operations_(),
      operations_(),
      all_mount_points_(),
      fs_types_(),
      bootloader_path_() {
  this->setObjectName("partition_delegate");

  partition_manager_->moveToThread(partition_thread_);
  this->initConnections();

  partition_thread_->start();
}

PartitionDelegate::~PartitionDelegate() {
  // Quit background thread explicitly.
  QuitThread(partition_thread_);
}

void PartitionDelegate::autoConf() {
  const QString script_path = GetAutoPartFile();
  emit partition_manager_->autoPart(script_path);
}

bool PartitionDelegate::canAddPrimary(const Partition& partition) const {
  const int index = DeviceIndex(devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = devices_.at(index);
  const PartitionList prim_partitions = GetPrimaryPartitions(device.partitions);
  const PartitionList logical_partitions = GetLogicalPartitions(
      device.partitions);

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

bool PartitionDelegate::canAddLogical(const Partition& partition) const {
  const int index = DeviceIndex(devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = devices_.at(index);

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

QStringList PartitionDelegate::getOperationDescriptions() const {
  QStringList descriptions;
  for (const Operation& operation : operations_) {
    descriptions.append(operation.description());
  }
  for (const Operation& operation : simple_operations_) {
    descriptions.append(operation.description());
  }

  qDebug() << "operation descriptions:" << descriptions;
  return descriptions;
}

Partition PartitionDelegate::getRealPartition(
    const Partition& virtual_partition) const {
  const int index = DeviceIndex(real_devices_, virtual_partition.device_path);
  if (index == -1) {
    qWarning() << "failed to find device:" << virtual_partition.device_path;
    return Partition();
  } else {
    for (const Partition& partition : real_devices_.at(index).partitions) {
      if (partition == virtual_partition) {
        return partition;
      }
    }
  }
  return Partition();
}

void PartitionDelegate::scanDevices() const {
  // If auto-part is not set, scan devices right now.
  if (!GetSettingsBool(kPartitionDoAutoPart)) {
    emit partition_manager_->refreshDevices(true);
  }
}

const QStringList& PartitionDelegate::allMountPoints() {
  if (all_mount_points_.isEmpty()) {
    // Read available mount points.
    all_mount_points_ = GetSettingsStringList(kPartitionMountPoints);

    // Replace "unused" mount point with ""
    for (QString& mount_point : all_mount_points_) {
      if (mount_point == kMountPointUnused) {
        mount_point = "";
      }
    }
  }
  return all_mount_points_;
}

const FsTypeList& PartitionDelegate::getFsTypes() {
  if (fs_types_.isEmpty()) {
    const QString name = GetSettingsString(kPartitionSupportedFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      FsType type = GetFsTypeByName(fs_name);
      fs_types_.append(type);
    }
  }
  return fs_types_;
}

void PartitionDelegate::doManualPart() {
  // Merge simple_operations_ and operations_.
  operations_.append(simple_operations_);

  // Set boot flags of boot partitions.
  bool is_boot_set = false;
  // First check EFI filesystem.
  for (Operation& operation : operations_) {
    if (operation.new_partition.fs == FsType::EFI) {
      operation.new_partition.flags.append(PartitionFlag::Boot);
      operation.new_partition.flags.append(PartitionFlag::ESP);
      is_boot_set = true;
    }
  }

  // Then, check /boot partition.
  if (!is_boot_set) {
    for (Operation& operation : operations_) {
      if (operation.new_partition.mount_point == kMountPointBoot) {
        operation.new_partition.flags.append(PartitionFlag::Boot);
        is_boot_set = true;
      }
    }
  }

  // At last, check / partition.
  if (!is_boot_set) {
    for (Operation& operation : operations_) {
      if (operation.new_partition.mount_point == kMountPointRoot) {
        operation.new_partition.flags.append(PartitionFlag::Boot);
        is_boot_set = true;
      }
    }
  }
  if (!is_boot_set) {
    // No boot partition found! Critical error!
    // We shall never reach here.
    qCritical() << "No boot partition found!";
    emit this->onManualPartDone(false);
  } else {
    emit partition_manager_->manualPart(operations_);
  }
}

void PartitionDelegate::createSimplePartition(const Partition& partition,
                                              PartitionType partition_type,
                                              bool align_start,
                                              FsType fs_type,
                                              const QString& mount_point,
                                              qint64 total_sectors) {
  if (partition_type == PartitionType::Normal) {
    createPrimaryPartition(partition,
                           align_start,
                           fs_type,
                           mount_point,
                           total_sectors,
                           true);
  } else if (partition_type == PartitionType::Logical) {
    createLogicalPartition(partition,
                           align_start,
                           fs_type,
                           mount_point,
                           total_sectors,
                           true);
  } else {
    qCritical() << "createSimplePartition() not supported type:"
                << partition_type;
  }
}

Partition PartitionDelegate::deleteSimplePartition(const Partition& partition) {
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

void PartitionDelegate::formatSimplePartition(const Partition& partition,
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
  simple_operations_.append(operation);
}

void PartitionDelegate::resetSimpleOperations() {
  simple_operations_.clear();
}

void PartitionDelegate::createPartition(const Partition& partition,
                                        PartitionType partition_type,
                                        bool align_start,
                                        FsType fs_type,
                                        const QString& mount_point,
                                        qint64 total_sectors) {
  if (partition_type == PartitionType::Normal) {
    createPrimaryPartition(partition,
                           align_start,
                           fs_type,
                           mount_point,
                           total_sectors,
                           false);
  } else if (partition_type == PartitionType::Logical) {
    createLogicalPartition(partition,
                           align_start,
                           fs_type,
                           mount_point,
                           total_sectors,
                           false);
  } else {
    qCritical() << "createPartition() not supported type:" << partition_type;
  }
}

void PartitionDelegate::deletePartition(const Partition& partition) {
  Partition new_partition;
  new_partition.sector_size = partition.sector_size;
  new_partition.start_sector = partition.start_sector;
  new_partition.end_sector = partition.end_sector;
  new_partition.device_path = partition.device_path;
  new_partition.type = PartitionType::Unallocated;
  new_partition.freespace = new_partition.length;
  new_partition.fs = FsType::Empty;
  new_partition.status = PartitionStatus::Delete;

  if (partition.status == PartitionStatus::New) {
    // If status of old partition is New, there shall be a CreateOperation
    // which generates that partition. Merge that CreateOperation
    // with DeleteOperation.
    for (int index = operations_.length() - 1; index >= 0; --index) {
      const Operation& operation = operations_.at(index);
      if (operation.type == OperationType::Create &&
          operation.new_partition == partition) {
        operations_.removeAt(index);
        break;
      }
    }
  } else {
    Operation operation(OperationType::Delete, partition, new_partition);
    operations_.append(operation);
  }

  if (partition.type == PartitionType::Logical) {
    // Delete extended partition if needed.
    const int device_index = DeviceIndex(devices_, partition.device_path);
    if (device_index == -1) {
      qCritical() << "deletePartition() Failed to get device:"
                  << partition.device_path;
    } else {
      const Device& device = devices_.at(device_index);
      const PartitionList logical_partitions =
          GetLogicalPartitions(device.partitions);
      if (logical_partitions.length() == 0) {
        qDebug() << "deletePartition() delete ext partition!";
        const int ext_index = ExtendedPartitionIndex(device.partitions);
        Q_ASSERT(ext_index > -1);
        Partition ext_partition = device.partitions.at(ext_index);
        qDebug() << "ext partition:" << ext_partition.path;
        Partition empty_partition = ext_partition;
        empty_partition.type = PartitionType::Unallocated;

        Operation delete_operation(OperationType::Delete, ext_partition,
                                   empty_partition);
        operations_.append(delete_operation);
      }
    }
  }
}

void PartitionDelegate::formatPartition(const Partition& partition,
                                        FsType fs_type,
                                        const QString& mount_point) {
  qDebug() << "formatPartition()" << partition.path << mount_point;

  this->resetOperationMountPoint(mount_point);

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
    // Update partition of old operation, instead of adding a new one.
    new_partition.status = partition.status;
    for (int index = operations_.length() - 1; index >= 0; --index) {
      Operation& operation = operations_[index];
      if (operation.type == OperationType::Format ||
          operation.type == OperationType::Create) {
        operation.new_partition = new_partition;
        return;
      }
    }
  }

  Operation operation(OperationType::Format, partition, new_partition);
  operations_.append(operation);
}

bool PartitionDelegate::unFormatPartition(const Partition& partition) {
  Q_ASSERT(partition.status == PartitionStatus::Format);
  if (partition.status == PartitionStatus::Format) {
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

void PartitionDelegate::updateMountPoint(const Partition& partition,
                                         const QString& mount_point) {
  qDebug() << "PartitionDelegate::updateMountPoint()" << partition.path
           << mount_point;

  // Reset mount-point of operation with the same mount-point.
  this->resetOperationMountPoint(mount_point);

  if (!mount_point.isEmpty()) {
    // Append MountPointOperation only if |mount_point| is not empty.
    Partition new_partition(partition);
    new_partition.mount_point = mount_point;
    // No need to update partition status.
    Operation operation(OperationType::MountPoint, partition, new_partition);
    operations_.append(operation);
  }
}

void PartitionDelegate::refreshVisual() {
  // Filters partition list based on the following policy:
  // * Remove extended partition if no logical partition exists;
  // * Merge unallocated partition with next unallocated one;
  // * Ignore partitions with size less than 100Mib;

  devices_ = real_devices_;
  for (Device& device : devices_) {
    PartitionList partitions;
    for (const Partition& partition : device.partitions) {
      if (partition.type == PartitionType::Normal ||
          partition.type == PartitionType::Logical ||
          partition.type == PartitionType::Extended) {
        partitions.append(partition);
      } else if (partition.type == PartitionType::Unallocated) {
        // Filters freespace.
        if (partition.getByteLength() > kMinimumPartitionSizeToDisplay) {
          partitions.append(partition);
        }
      }
    }
    device.partitions = partitions;
  }

//  for (Device& device : devices_) {
//    PartitionList new_partitions;
//    const PartitionList& old_partitions = device.partitions;
//    Partition unallocated_partition;
//    for (int index = 0; index < old_partitions.length(); ) {
//      if (old_partitions.at(index).type == PartitionType::Unallocated) {
//        unallocated_partition = old_partitions.at(index);
//        index ++;
//
//        while (index < old_partitions.length()) {
//          if (old_partitions.at(index).type == PartitionType::Unallocated) {
//            // Merge unallocated partitions.
//            qDebug() << "merge partition:" << old_partitions.at(index);
//            unallocated_partition.end_sector = old_partitions.at(index).end_sector;
//            index ++;
//          } else if (old_partitions.at(index).type == PartitionType::Extended) {
//            // Ignores extended partition.
//            index ++;
//          } else {
//            break;
//          }
//        }
//
//        // Ignore unallocated partition if it is too small.
//        if (unallocated_partition.getByteLength() > kMinimumPartitionSizeToDisplay) {
//          new_partitions.append(unallocated_partition);
//        }
//
//      } else {
//        new_partitions.append(old_partitions.at(index));
//        index ++;
//      }
//    }
//    device.partitions = new_partitions;
//  }

  for (Device& device : devices_) {
    for (Operation& operation : operations_) {
      if (operation.orig_partition.device_path == device.path) {
        operation.applyToVisual(device.partitions);
      }
    }
  }

  emit this->deviceRefreshed();

  qDebug() << "operations:" << operations_;
}

void PartitionDelegate::setBootloaderPath(const QString bootloader_path) {
  bootloader_path_ = bootloader_path;
}

void PartitionDelegate::initConnections() {
  connect(partition_manager_, &PartitionManager::autoPartDone,
          this, &PartitionDelegate::autoPartDone);
  connect(partition_manager_, &PartitionManager::manualPartDone,
          this, &PartitionDelegate::onManualPartDone);
  connect(partition_manager_, &PartitionManager::devicesRefreshed,
          this, &PartitionDelegate::onDevicesRefreshed);

  connect(partition_thread_, &QThread::finished,
          partition_manager_, &PartitionManager::deleteLater);
}

void PartitionDelegate::createPrimaryPartition(const Partition& partition,
                                               bool align_start,
                                               FsType fs_type,
                                               const QString& mount_point,
                                               qint64 total_sectors,
                                               bool is_simple) {
  // Policy:
  // * If new primary partition is contained in or intersected with
  //   extended partition, shrink extended partition or delete it if no other
  //   logical partitions.

  const int device_index = DeviceIndex(devices_, partition.device_path);
  if (device_index == -1) {
    // TODO(xushaohua): Raise error msg
    qCritical() << "createPartition() device index out of range:"
                << partition.device_path;
    return;
  }
  const Device& device = devices_.at(device_index);

  // TODO(xushaohua): Handles extended partition here.

  Partition new_partition;
  new_partition.device_path = partition.device_path;
  new_partition.path = partition.path;
  new_partition.sector_size = partition.sector_size;
  new_partition.status = PartitionStatus::New;
  new_partition.type = PartitionType::Normal;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  const int partition_number = AllocPrimaryPartitionNumber(device);
  Q_ASSERT(partition_number > -1);
  new_partition.changeNumber(partition_number);

  // Check whether space is required for the Master Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition.
  const qint64 oneMebiByteSector = 1 * kMebiByte / partition.sector_size;
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

  Q_ASSERT(partition.start_sector <= new_partition.start_sector);
  Q_ASSERT(new_partition.start_sector < new_partition.end_sector);
  Q_ASSERT(new_partition.end_sector <= partition.end_sector);

  if (is_simple) {
    Operation operation(OperationType::Create, partition, new_partition);
    simple_operations_.append(operation);
  } else {
    // Reset mount-point before appending operation to advanced operation list.
    this->resetOperationMountPoint(mount_point);
    Operation operation(OperationType::Create, partition, new_partition);
    operations_.append(operation);
  }
}

void PartitionDelegate::createLogicalPartition(const Partition& partition,
                                               bool align_start,
                                               FsType fs_type,
                                               const QString& mount_point,
                                               qint64 total_sectors,
                                               bool is_simple) {
  // Policy:
  // * Create extended partition if not found;
  // * If new logical partition is not contained in or is intersected with
  //   extended partition, enlarge extended partition.

  const int device_index = DeviceIndex(devices_, partition.device_path);
  if (device_index == -1) {
    // TODO(xushaohua): Raise error msg
    qCritical() << "createPartition() device index out of range:"
                << partition.device_path;
    return;
  }
  const Device& device = devices_.at(device_index);

  Partition new_partition;
  new_partition.device_path = partition.device_path;
  new_partition.path = partition.path;
  new_partition.sector_size = partition.sector_size;
  new_partition.status = PartitionStatus::New;
  new_partition.type = PartitionType::Logical;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  const int partition_number = AllocLogicalPartitionNumber(device);
  // FIXME(xushaohua): crashes here
  Q_ASSERT(partition_number > -1);
  new_partition.changeNumber(partition_number);

  // space is required for the Extended Boot Record.
  // Generally an additional track or MebiByte is required so for
  // our purposes reserve a MebiByte in front of the partition.
  const qint64 oneMebiByteSector = 1 * kMebiByte / partition.sector_size;
  if (align_start) {
    // Align from start of |partition|.
    new_partition.start_sector = partition.start_sector + oneMebiByteSector;
    new_partition.end_sector = qMin(partition.end_sector,
        total_sectors + new_partition.start_sector - 1);
  } else {
    new_partition.end_sector = partition.end_sector;
    new_partition.start_sector = qMax(
        partition.start_sector + oneMebiByteSector,
        partition.end_sector - total_sectors + 1);
  }

  // No need to add extended partition or enlarge it.
  const int ext_index = ExtendedPartitionIndex(device.partitions);
  Partition ext_partition;
  if (ext_index == -1) {
    // No extended partition found, create one.
    ext_partition.device_path = new_partition.device_path;
    ext_partition.changeNumber(AllocPrimaryPartitionNumber(device));
    ext_partition.type = PartitionType::Extended;
    ext_partition.start_sector = new_partition.start_sector;
    ext_partition.end_sector = new_partition.end_sector;
    ext_partition.sector_size = new_partition.sector_size;
    Q_ASSERT(ext_partition.partition_number > 0);
    Operation ext_operation(OperationType::Create, partition, ext_partition);
    operations_.append(ext_operation);

    // Add space for Extended Boot Record.
    new_partition.start_sector = new_partition.start_sector + oneMebiByteSector;
  } else {
    ext_partition = device.partitions.at(ext_index);
  }
  
  Q_ASSERT(partition.start_sector <= new_partition.start_sector);
  Q_ASSERT(new_partition.start_sector < new_partition.end_sector);
  Q_ASSERT(new_partition.end_sector <= partition.end_sector);

  // Enlarge extended partition if needed.
  if (ext_partition.start_sector > new_partition.start_sector ||
      ext_partition.end_sector < new_partition.end_sector) {
    Partition new_ext_partition(ext_partition);
    new_ext_partition.start_sector = qMin(ext_partition.start_sector,
                                          new_partition.start_sector);
    new_ext_partition.end_sector = qMax(ext_partition.end_sector,
                                        new_partition.end_sector);
    Operation resize_ext_operation(OperationType::Resize, ext_partition,
                                   new_ext_partition);
    operations_.append(resize_ext_operation);
  }

  if (is_simple) {
    Operation operation(OperationType::Create, partition, new_partition);
    simple_operations_.append(operation);
  } else {
    // Reset mount-point before append operation to advanced operation list.
    this->resetOperationMountPoint(mount_point);
    Operation operation(OperationType::Create, partition, new_partition);
    operations_.append(operation);
  }
}

void PartitionDelegate::removeEmptyExtendedPartition(
    const PartitionList& partitions) {
  const int ext_index = ExtendedPartitionIndex(partitions);
  if (ext_index > -1) {
    const PartitionList logical_parts = GetLogicalPartitions(partitions);
    if (logical_parts.length() == 0) {
      Partition ext_partition = partitions.at(ext_index);
      Partition new_partition;
      Operation operation(OperationType::Delete, ext_partition, new_partition);
      operations_.append(operation);
    }
  }
}

void PartitionDelegate::resetOperationMountPoint(const QString& mount_point) {
  for (int index = operations_.length() - 1; index >= 0; --index) {
    Operation& operation = operations_[index];
    if (operation.new_partition.mount_point == mount_point) {
      if (operation.type == OperationType::MountPoint) {
        // Remove MountPointOperation with same mount point.
        operations_.removeAt(index);
        return;
      }
      // Clear mount point of old operation.
      operation.new_partition.mount_point = "";
      return;
    }
  }
}

void PartitionDelegate::onDevicesRefreshed(const DeviceList& devices) {
  qDebug() << "device refreshed:" << devices;
  this->real_devices_ = devices;

  // Clear operation list.
  simple_operations_.clear();
  operations_.clear();

  emit this->realDeviceRefreshed();

  this->refreshVisual();
}

void PartitionDelegate::onManualPartDone(bool ok) {
  if (ok) {
    QString root_disk;
    QString root_path;
    QStringList mount_points;

    for (const Operation& operation : operations_) {
      const Partition& new_partition = operation.new_partition;

      if (!new_partition.mount_point.isEmpty()) {
        // Add used partitions to mount_point list.
        mount_points.append(QString("%1=%2").arg(new_partition.path)
                                            .arg(new_partition.mount_point));
        if (new_partition.mount_point == kMountPointRoot) {
          root_disk = new_partition.device_path;
          root_path = new_partition.path;
        }
      } else if (new_partition.fs == FsType::LinuxSwap) {
        // Add swap area to mount_point list.
        mount_points.append(QString("%1=swap").arg(new_partition.path));
      }
    }

    if (IsEfiEnabled()) {
      // Enable EFI mode.
      // First check newly created EFI partition. If not found, check existing
      // EFI partition.
      WriteUEFI(true);
      QString esp_path;
      for (const Operation& operation : operations_) {
        if (operation.new_partition.fs == FsType::EFI) {
          // NOTE(xushaohua): There shall be only one EFI partition.
          esp_path = operation.new_partition.path;
          break;
        }
      }
      if (esp_path.isEmpty()) {
        // If no new EFI partition is created, search in device list.
        bool esp_found = false;
        for (const Device& device : devices_) {
          for (const Partition& partition : device.partitions) {
            if (partition.fs == FsType::EFI) {
              esp_path = partition.path;
              esp_found = true;
              break;
            }
          }

          if (esp_found) {
            break;
          }
        }
      }

      if (esp_path.isEmpty()) {
        qCritical() << "esp path is empty!";
      }
      WritePartitionInfo(root_disk, root_path, esp_path,
                         mount_points.join(';'));
    } else {
      // In legacy mode.
      WritePartitionInfo(root_disk, root_path, bootloader_path_,
                         mount_points.join(';'));
    }
  }

  emit this->manualPartDone(ok);
}

}  // namespace installer
