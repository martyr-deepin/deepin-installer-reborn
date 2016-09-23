// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_LIBPARTED_UTIL_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_LIBPARTED_UTIL_H

#include <parted/parted.h>
#include <QString>

namespace partman {

bool CommitDiskChanges(PedDisk* disk);

// Destroy libparted-device and disk object.
void DestroyDeviceAndDisk(PedDevice* device, PedDisk* disk);

// Flush linux kernel caches. Ensure coherency between the caches of the whole
// disk device and the partition devices.
bool FlushDevice(PedDevice* device);

// Get libparted-device and disk object at |path|.
// Call |DestroyDeviceAndDisk()| to release them.
bool GetDeviceAndDisk(const QString& path, PedDevice* device, PedDisk* disk);

// Refers: http://stackoverflow.com/questions/14127210/
// After the kernel boots, `udevd` is used to create device nodes for
// all detected devices. That is a relatively time consuming task that
// has to be completed for the boot process to continue, otherwise
// there is a risk of services failing due to missing device nodes.
// `udevadm settle` waits for udevd to process the device creation events
// for all hardware devices, thus ensuring that any device nodes
// have been created successfully before proceeding.
void SettleDevice(int timeout);

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_LIBPARTED_UTIL_H
