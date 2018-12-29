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

namespace installer {
namespace {

TEST(Operation, MergeUnallocatedPartitions) {
  PartitionList partitions;
  {
    Partition::Ptr partition(new Partition);
    partition->type = PartitionType::Unallocated;
    partition->start_sector = 63;
    partition->end_sector = 1000;
    partitions.append(partition);
  }

  {
    Partition::Ptr partition(new Partition);
    partition->type = PartitionType::Unallocated;
    partition->start_sector = 1001;
    partition->end_sector = 2000;
    partitions.append(partition);
  }

  {
    Partition::Ptr partition(new Partition);
    partition->type = PartitionType::Normal;
    partition->start_sector = 2001;
    partition->end_sector = 3000;
    partitions.append(partition);
  }

  {
    Partition::Ptr partition(new Partition);
    partition->type = PartitionType::Unallocated;
    partition->start_sector = 3001;
    partition->end_sector = 4000;
    partitions.append(partition);
  }

  {
    Partition::Ptr partition(new Partition);
    partition->type = PartitionType::Unallocated;
    partition->start_sector = 4001;
    partition->end_sector = 5000;
    partitions.append(partition);
  }

  MergeUnallocatedPartitions(partitions);

  EXPECT_EQ(partitions.length(), 3);
}

}  // namespace
}  // namespace installer
