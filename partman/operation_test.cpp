// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

TEST(OperationTest, ApplyToDiskDelete) {
  Partition partition;
  partition.sector_start = 2048;
  partition.sector_end = 21256191;
  partition.fs = FsType::Ext4;
  partition.device_path = "/dev/sdc";
  partition.path = "/dev/sdc1";
  partition.type = PartitionType::Primary;
  Operation format_operation(OperationType::Delete, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, ApplyToDiskCreate) {
  Partition partition;
  partition.sector_start = 2048;
  partition.sector_end = 21256191;
  partition.fs = FsType::Ext4;
  partition.device_path = "/dev/sdc";
  partition.path = "/dev/sdc1";
  partition.type = PartitionType::Primary;
  Operation format_operation(OperationType::Create, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

TEST(OperationTest, ApplyToDiskFormat) {
  Partition partition;
  partition.sector_start = 2048;
  partition.sector_end = 21256191;
  partition.fs = FsType::Ext4;
  partition.device_path = "/dev/sdc";
  partition.path = "/dev/sdc1";
  partition.label = "test label with spaces paddddddd!";
  partition.type = PartitionType::Primary;
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

}  // namespace
}  // namespace partman