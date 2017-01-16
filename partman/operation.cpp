// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include <QDebug>

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace installer {

QDebug& operator<<(QDebug& debug, const OperationType& op_type) {
  QString type;
  switch (op_type) {
    case OperationType::Create: {
      type = "Create";
      break;
    }
    case OperationType::Delete: {
      type = "Delete";
      break;
    }
    case OperationType::Format: {
      type = "Format";
      break;
    }
    case OperationType::MountPoint: {
      type = "MountPoint";
      break;
    }
    case OperationType::NewPartTable: {
      type = "NewPartTable";
      break;
    }
    case OperationType::Resize: {
      type = "Resize";
      break;
    }
    case OperationType::Invalid: {
      type = "Invalid";
      break;
    }
  }
  debug << type;
  return debug;
}

Operation::Operation(OperationType type,
                     const Partition& orig_partition,
                     const Partition& new_partition)
    : type(type),
      orig_partition(orig_partition),
      new_partition(new_partition) {
}

Operation::~Operation() {
}

bool Operation::applyToDisk() const {
  bool ok;
  switch (type) {
    case OperationType::Create: {
      // TODO(xushaohua): Update partition number.
      ok = CreatePartition(new_partition);
      qDebug() << "applyToDisk() create partition:" << ok;
      // Ignores extended partition.
      // TODO(xushaohua): Check new_partition.fs
      if (ok && new_partition.type != PartitionType::Extended) {
        ok = Mkfs(new_partition);
        qDebug() << "applyToDisk() mkfs:" << ok;
      }
      break;
    }

    case OperationType::Delete: {
      ok = DeletePartition(orig_partition);
      qDebug() << "applyToDisk() delete partition" << ok;
      break;
    }

    case OperationType::Format: {
      ok = SetPartitionType(new_partition);
      qDebug() << "applyToDisk() format set partition type:" << ok;
      if (ok) {
        ok = Mkfs(new_partition);
        qDebug() << "applyToDisk() format mkfs():" << ok;
      }
      break;
    }

    case OperationType::Resize: {
      ok = ResizeMovePartition(new_partition);
      qDebug() << "applyToDisk() resize partition:" << ok;
      break;
    }

    default: {
      ok = false;
      break;
    }
  }

  // Set flags of new_partition.
  if (ok) {
    ok = SetPartitionFlags(new_partition);
    qDebug() << "Set partition flags:" << new_partition.path << ok;
  }

  return ok;
}

void Operation::applyToVisual(PartitionList& partitions) const {
  switch (type) {
    case OperationType::Create: {
      this->applyCreateVisual(partitions);
      break;
    }
    case OperationType::Delete: {
      this->applyDeleteVisual(partitions);
      break;
    }
    case OperationType::Format: {
      this->substitute(partitions);
      break;
    }
    case OperationType::MountPoint: {
      this->substitute(partitions);
      break;
    }
    case OperationType::Resize: {
      this->applyResizeVisual(partitions);
      break;
    }
    default: {
      break;
    }
  }
}

QString Operation::description() const {
  QString desc;
  switch (type) {
    case OperationType::Create: {
      if (new_partition.type == PartitionType::Extended) {
        desc = QObject::tr("Create extended partition %1")
            .arg(new_partition.path);
      }
      else if (new_partition.mount_point.isEmpty()) {
        desc = QObject::tr("Create partition %1 with %2")
            .arg(new_partition.path)
            .arg(GetFsTypeName(new_partition.fs));
      } else {
        desc = QObject::tr("Create partition %1 for %2 with %3")
            .arg(new_partition.path)
            .arg(new_partition.mount_point)
            .arg(GetFsTypeName(new_partition.fs));
      }
      break;
    }
    case OperationType::Delete: {
      desc = QObject::tr("Delete partition %1").arg(orig_partition.path);
      break;
    }
    case OperationType::Format: {
      if (new_partition.mount_point.isEmpty()) {
        desc = QObject::tr("Format %1 with %2")
            .arg(new_partition.path)
            .arg(GetFsTypeName(new_partition.fs));
      } else {
        desc = QObject::tr("Format %1 for %2 with %3")
            .arg(new_partition.path)
            .arg(new_partition.mount_point)
            .arg( GetFsTypeName(new_partition.fs));
      }
      break;
    }
    case OperationType::MountPoint: {
      desc = QObject::tr("Use %1 for %2")
          .arg(new_partition.path)
          .arg(new_partition.mount_point);
      break;
    }
    case OperationType::Resize: {
      desc = QObject::tr("Resize partition %1").arg(new_partition.path);
      break;
    }
    default: {
      // pass
      break;
    }
  }
  return desc;
}

void Operation::applyCreateVisual(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  if (index == -1) {
    qCritical() << "applyCreateVisual() Failed to find partition:"
                << orig_partition.path;
    return;
  }

  // Do not remove orig partition when creating extended partition.
  if (new_partition.type == PartitionType::Extended) {
    partitions.insert(index, new_partition);
  } else {
    partitions[index] = new_partition;
  }

  // TODO(xushaohua): Merge unallocated spaces.
}

void Operation::applyDeleteVisual(PartitionList& partitions) const {
  // Policy:
  // * Update partition number if logical partition is deleted;
  // * Merge unallocated partitions;

  int index = PartitionIndex(partitions, orig_partition);
  Q_ASSERT(index > -1);

  if (orig_partition.type == PartitionType::Logical) {
    // Update partition number of logical partitions.
    for (Partition& partition : partitions) {
      if (partition.type == PartitionType::Logical &&
          partition.partition_number > orig_partition.partition_number) {
        partition.changeNumber(partition.partition_number - 1);
      }
    }
  }

  partitions[index] = new_partition;
  // TODO(xushaohua): Merge unallocated spaces.
}

void Operation::applyResizeVisual(PartitionList& partitions) const {
  // Currently only extended partition is allowed to resize
  Q_ASSERT(new_partition.type == PartitionType::Extended);
  this->substitute(partitions);
}

void Operation::substitute(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  if (index == -1) {
    qCritical() << "substitute() Failed to find partition:"
                << orig_partition.path;
  } else {
    partitions[index] = new_partition;
  }
}

QDebug& operator<<(QDebug& debug, const Operation& operation) {
  debug << "Operation: {"
        << "type:" << operation.type
        << "orig_partition:" << operation.orig_partition
        << "new_partition:" << operation.new_partition
        << "}";
  return debug;
}

void MergeOperations(OperationList& operations, const Operation& operation) {
  Q_UNUSED(operations);
  Q_UNUSED(operation);
}

}  // namespace installer