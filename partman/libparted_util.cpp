// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/libparted_util.h"

#include "base/command.h"

namespace partman {

bool CommitDiskChanges(PedDisk* disk) {
  bool success = static_cast<bool>(ped_disk_commit_to_dev(disk));
  if (success) {
    success = static_cast<bool>(ped_disk_commit_to_os(disk));
  }
  return success;
}

void DestroyDeviceAndDisk(PedDevice* device, PedDisk* disk) {
  if (device) {
    ped_device_destroy(device);
    device = NULL;
  }

  if (disk) {
    ped_disk_destroy(disk);
    disk = NULL;
  }
}

bool FlushDevice(PedDevice* device) {
  bool success = false;
  if (ped_device_open(device)) {
    success = static_cast<bool>(ped_device_sync(device));
    ped_device_close(device);
  }
  return success;
}

bool GetDeviceAndDisk(const QString& path, PedDevice* device, PedDisk* disk) {
  device = ped_device_get(path.toLatin1().data());
  if (device) {
    disk = ped_disk_new(device);
    if (disk) {
      return true;
    } else {
      DestroyDeviceAndDisk(device, disk);
      return false;
    }
  } else {
    return false;
  }
}

void SettleDevice(int timeout) {
  base::SpawnCmd("udevadm", {"settle", QString("--timeout=%1").arg(timeout)});
}

}  // namespace partman