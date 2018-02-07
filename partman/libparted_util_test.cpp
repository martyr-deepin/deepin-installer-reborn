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