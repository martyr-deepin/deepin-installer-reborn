// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/device.h"

namespace partman {

Device::Device() {
  this->reset();
}

void Device::reset() {
  partitions.clear();
  length = 0;
  heads = 0;
  sectors = 0;
  cylinders = 0;
  cylsize = 0;
  model = "";
  path = "";
  disk_type = "";
  sector_size = 0;
  max_prims = 0;
  read_only = false;
}

}  // namespace partman