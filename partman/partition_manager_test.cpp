// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_manager.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(PartitionManagerTest, ScanDevices) {
  const DeviceList devices = ScanDevices();
  EXPECT_FALSE(devices.isEmpty());
}

}  // namespace
}  // namespace installer