// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_PARTITION_UTIL_H
#define INSTALLER_UI_DELEGATES_PARTITION_UTIL_H

#include <QtCore/QtGlobal>

#include "partman/device.h"
#include "partman/fs.h"
#include "partman/partition.h"

namespace installer {

// Align partition to nearest mebibytes.
void AlignPartition(Partition& partition);

// Calculate new logical partition number.
int AllocLogicalPartitionNumber(const Device& device);

// Calculate new primary partition number. Returns -1 on failed.
int AllocPrimaryPartitionNumber(const Device& device);

// Filters installer device disk from |devices|.
DeviceList FilterInstallerDevice(const DeviceList& devices);

// Get default filesystem from settings.
FsType GetDefaultFsType();

// Get device model name and capacity.
QString GetDeviceModelAndCap(const Device& device);

// Get device model name, disk capacity and path.
QString GetDeviceModelCapAndPath(const Device& device);

// Get device path which is mounted at /lib/live/mount/medium.
// It might be a partition path or a real device path.
// Returns an empty string if not found.
QString GetInstallerDevicePath();

// Returns human readable file system name.
QString GetLocalFsTypeName(FsType fs_type);

// Get icon path of os type
QString GetOsTypeIcon(OsType os_type);
QString GetOsTypeLargeIcon(OsType os_type);

// Get partition name based on |path|.
QString GetPartitionLabel(const Partition& partition);
QString GetPartitionLabelAndPath(const Partition& partition);
QString GetPartitionName(const QString& path);

// Get human readable partition usage.
QString GetPartitionUsage(const Partition& partition);

// Get partition usage percentage (0-100).
int GetPartitionUsageValue(const Partition& partition);

// Check whether specific fs type can be mounted by user.
// linux-swap and efi are mounted at fixed position and thus returns false.1
bool IsMountPointSupported(FsType fs_type);

// Check whether partition table matches machine settings.
bool IsPartitionTableMatch(PartitionTableType type);
// Check whether partition table of device at |device_path| in |devices|
// matches current machine state.
bool IsPartitionTableMatch(const DeviceList& devices,
                           const QString& device_path);

// If physical memory is less than 4Gib, swap partition of swap file is needed.
bool IsSwapAreaNeeded();

// Convert |size| in byte to gibibyte.
int ToGigByte(qint64 size);

// Convert |size| in byte to gibibyte.
int ToMebiByte(qint64 size);

// Trim |text| length to |max_len|.
QString TrimText(const QString& text, int max_len);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_PARTITION_UTIL_H
