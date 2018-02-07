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

#include "partman/operation.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

const char kLongLabel[] = "LONG * Label with special chars";

static Partition GetTestPartition() {
  Partition partition;
  partition.fs = FsType::Empty;
  partition.sector_start = 2048;
  partition.sector_end = 21256191;
  partition.type = PartitionType::Primary;
  partition.device_path = "/dev/sdc";
  partition.path = "/dev/sdc1";
  return partition;
}

TEST(OperationTest, ApplyToDiskDelete) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext4;
  Operation format_operation(OperationType::Delete, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, ApplyToDiskCreate) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext4;
  Operation format_operation(OperationType::Create, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, ApplyToDiskFormat) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext4;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatBtrfs) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Btrfs;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatBtrfsWithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Btrfs;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatExt2) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext2;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatExt2WithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext2;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatExt4) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext4;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatExt4WithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Ext4;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatFat16) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Fat16;
  Operation format_operation(OperationType::Format, partition, partition);
  // Maximum filesystem size supported by fat16 is 4Gib, so mkfs.msdos
  // always returns false.
  EXPECT_FALSE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatFat16WithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Fat16;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_FALSE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatFat32) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Fat32;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatFat32WithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Fat32;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatJfs) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Jfs;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatJfsWithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Jfs;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatLinuxSwap) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::LinuxSwap;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatLinuxSwapWithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::LinuxSwap;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatNTFS) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::NTFS;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatNTFSWithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::NTFS;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatXfs) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Xfs;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, FormatXfsWithLabel) {
  Partition partition = GetTestPartition();
  partition.fs = FsType::Xfs;
  partition.label = kLongLabel;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

}  // namespace
}  // namespace partman