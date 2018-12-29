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

#include "partman/partition.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(Partition, IsPartitionsJoint) {
  Partition::Ptr part1 (new Partition);
  Partition::Ptr part2 (new Partition);

  part1->start_sector = 1024;
  part1->end_sector = 3000;
  part2->start_sector = 2048;
  part2->end_sector = 4096;
  EXPECT_TRUE(IsPartitionsJoint(part1, part2));

  part1->start_sector = 5000;
  part1->end_sector = 6000;
  EXPECT_FALSE(IsPartitionsJoint(part1, part2));

  part1->start_sector = 3000;
  part1->end_sector = 5000;
  EXPECT_TRUE(IsPartitionsJoint(part1, part2));
}

}  // namespace
}  // namespace installer
