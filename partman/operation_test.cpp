// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <parted/parted.h>

#include "partman/operation.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

TEST(OperationTest, ApplyToDisk) {
  Partition partition;
  partition.sector_start = 305072128;
  partition.sector_end = 367165439;
  partition.fs = FsType::Ext4;
  partition.path = "/dev/sdc2";
  Operation format_operation(OperationType::Format, partition, partition);
  EXPECT_TRUE(format_operation.applyToDisk());
}

}  // namespace
}  // namespace partman