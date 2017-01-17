// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(Partition, IsPartitionsJoint) {
  Partition part1;
  Partition part2;

  part1.start_sector = 1024;
  part1.end_sector = 3000;
  part2.start_sector = 2048;
  part2.end_sector = 4096;
  EXPECT_TRUE(IsPartitionsJoint(part1, part2));

  part1.start_sector = 5000;
  part1.end_sector = 6000;
  EXPECT_FALSE(IsPartitionsJoint(part1, part2));

  part1.start_sector = 3000;
  part1.end_sector = 5000;
  EXPECT_TRUE(IsPartitionsJoint(part1, part2));
}

}  // namespace
}  // namespace installer