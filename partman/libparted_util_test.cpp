// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/libparted_util.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(LibPartedUtil, SetPartitionFlags) {
  Partition partition;
  partition.start_sector = 419432448;
  partition.end_sector = 434227199;
  partition.sector_size = 512;
  partition.path = "/dev/sdb3";
  partition.device_path = "/dev/sdb";
  partition.flags.append(PartitionFlag::Boot);
  partition.flags.append(PartitionFlag::ESP);
  EXPECT_TRUE(SetPartitionFlags(partition));
}

}  // namespace
}  // namespace installer