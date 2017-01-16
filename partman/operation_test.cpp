// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(Operation, MergeUnallocatedPartitions) {
  PartitionList partitions;
  {
    Partition partition;
    partition.type = PartitionType::Unallocated;
    partition.start_sector = 63;
    partition.end_sector = 1000;
    partitions.append(partition);
  }

  {
    Partition partition;
    partition.type = PartitionType::Unallocated;
    partition.start_sector = 1001;
    partition.end_sector = 2000;
    partitions.append(partition);
  }

  {
    Partition partition;
    partition.type = PartitionType::Normal;
    partition.start_sector = 2001;
    partition.end_sector = 3000;
    partitions.append(partition);
  }

  {
    Partition partition;
    partition.type = PartitionType::Unallocated;
    partition.start_sector = 3001;
    partition.end_sector = 4000;
    partitions.append(partition);
  }

  {
    Partition partition;
    partition.type = PartitionType::Unallocated;
    partition.start_sector = 4001;
    partition.end_sector = 5000;
    partitions.append(partition);
  }

  MergeUnallocatedPartitions(partitions);

  EXPECT_EQ(partitions.length(), 3);
}

}  // namespace
}  // namespace installer