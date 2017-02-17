// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/libparted_util.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {
//
//TEST(LibPartedUtil, SetPartitionFlags) {
//  Partition partition;
//  partition.start_sector = 104859648;
//  partition.end_sector = 113281023;
//  partition.sector_size = 512;
//  partition.path = "/dev/sdb2";
//  partition.device_path = "/dev/sdb";
//  partition.flags.append(PartitionFlag::Boot);
//  EXPECT_TRUE(SetPartitionFlags(partition));
//}
//
//TEST(LibPartedUtil, ResizeMovePartition) {
//  Partition partition;
//  // Prepend 2GiB.
//  partition.start_sector = 179640320;
//  // Append 1GiB.
//  partition.end_sector = 249995263;
//  partition.type = PartitionType::Extended;
//  partition.sector_size = 512;
//  partition.path = "/dev/sdb3";
//  partition.device_path = "/dev/sdb";
//  EXPECT_TRUE(ResizeMovePartition(partition));
//}

TEST(LibPartedUtil, CreatePartitionTable) {
  const QString device_path("/dev/sdb");
  PartitionTableType table = PartitionTableType::GPT;
  EXPECT_TRUE(CreatePartitionTable(device_path, table));
}

}  // namespace
}  // namespace installer