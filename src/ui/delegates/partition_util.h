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

#ifndef INSTALLER_UI_DELEGATES_PARTITION_UTIL_H
#define INSTALLER_UI_DELEGATES_PARTITION_UTIL_H

#include <QtCore/QtGlobal>

#include "partman/device.h"
#include "partman/fs.h"
#include "partman/partition.h"

namespace installer {

// Align partition to nearest mebibytes.
void AlignPartition(Partition::Ptr partition);

// Calculate new logical partition number.
int AllocLogicalPartitionNumber(const Device::Ptr device);

// Calculate new primary partition number. Returns -1 on failed.
int AllocPrimaryPartitionNumber(const Device::Ptr device);

// Filters installer device disk from |devices|.
DeviceList FilterInstallerDevice(const DeviceList& devices);

// Filters fragmentation partition from devices
PartitionList FilterFragmentationPartition(PartitionList partitionList);

// Get default filesystem from settings.
FsType GetDefaultFsType();

// Get device model name and capacity.
QString GetDeviceModelAndCap(const Device::Ptr device);

// Get device model name, disk capacity and path.
QString GetDeviceModelCapAndPath(const Device::Ptr device);

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
QString GetPartitionLabel(const Partition::Ptr partition);
QString GetPartitionLabelAndPath(const Partition::Ptr partition);
QString GetPartitionName(const QString& path);

// Get human readable partition usage.
QString GetPartitionUsage(const Partition::Ptr partition);

// Get partition usage percentage (0-100).
int GetPartitionUsageValue(const Partition::Ptr partition);

// If target disk has msdos partition table, but system boots up in UEFI mode,
// ignore this UEFI mode if any systems is found in that disk.
bool IgnoreUEFI(const DeviceList& devices);

// Check if EFI feature is enabled in this machine.
bool IsEfiEnabled();

// Returns true if current boot mode is mbr or any system is found on disks
// with msdos partition table.
bool IsMBRPreferred(const DeviceList& devices);

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

// create empty partition
Partition::Ptr createEmptyPartition(QString device_path, qint64 sector_size, qint64 start_sector, qint64 end_sector);

/**
 * Parse partition size from string.
 * @param size like used in parted, e.g., 100Mib, 2Gib, 10%
 * @param device_length Size of device in which this partition is located.
 * @return actual size of partition, in bytes. Returns -1 on error.
 */
qint64 ParsePartitionSize(const QString& size, qint64 device_length);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_PARTITION_UTIL_H
