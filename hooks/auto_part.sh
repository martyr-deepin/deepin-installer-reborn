#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Automatically create disk partitions based on this policy:
# Assumes /dev/sda is the device which has the largest capacity.
# If UEFI is enabled:
#   * Create label of /dev/sda to GPT
#   If /dev/sda is less than 15G:
#     * /dev/sda1 /boot/efi efi 512M
#     * /dev/sda2 / ext4
#   Else if /dev/sda is less than 60G:
#     * /dev/sda1 /boot/efi efi 512M
#     * /dev/sda2 / ext4
#     * /dev/sda3 linux-swap 4G
#   Else:
#     * /dev/sda1 /boot/efi efi 512M
#     * /dev/sda2 / ext4 30G
#     * /dev/sda3 linux-swap 4G
#     * /dev/sda4 /home ext4
#   EndIf
# Else:
#   * Create label of /dev/sda to msdos
#   If /dev/sda is less than 15G:
#     * /dev/sda1 / ext4
#   Else if /dev/sda is less than 60G:
#     * /dev/sda1 / ext4
#     * /dev/sda2 linux-swap 4G
#   Else:
#     * /dev/sda1 / ext4 30G
#     * /dev/sda2 linux-swap 4G
#     * /dev/sda3 /home ext4
#   EndIf
# EndIf

kGibiByte=1048576
k4Gib=4096
k15Gib=15360
k30Gib=30720
k60Gib=61440

# Minimum disk capacity required, 15G.
MINIMUM_DISK_SIZE=$k15Gib

# The disk with largest storage capacity is used as system device.
get_max_capacity_device() {
  DEVICE=''
  MAX_CAPACITY=0
  lsblk -ndb -o NAME,SIZE 2>/dev/null | (while read NAME SIZE; do
    #echo $NAME:$SIZE
    #echo $DEVICE:$MAX_CAPACITY
    if [ $MAX_CAPACITY -lt $SIZE ]; then
      MAX_CAPACITY=$SIZE
      DEVICE=$NAME
    fi
  done && echo /dev/$DEVICE)
}

# Flush kernel message
flush_message() {
  udevadm settle --timeout=5
}

# Create a EFI partition on /dev/sda1 with 512M
make_efi() {
  parted -s $DEVICE mkpart primary fat32 1Mib 512Mib || \
    error "Failed to create partition ${DEVICE}1"
  parted -s $DEVICE set 1 esp on || \
    error "Failed to set esp flag on ${DEVICE}1"
  flush_message
  mkfs.msdos -F32 -v ${DEVICE}1 || \
    error "Failed to make fat32 filesystem on ${DEVICE}1"
}

# Add boot flag to ${DEVICE}1
mark_device1_bootable() {
  parted -s $DEVICE set 1 boot on || \
    error "Failed to set esp flag on ${DEVICE}1"
}

# Umount all swap partitions.
swapoff -a || error "Failed to umount swap!"

DEVICE=$(get_max_capacity_device)
if [ -z $DEVICE ]; then
  echo 'Error: no supported storage device found!';
  echo 'There shall be a IDE or SATA disk available at /dev/sda or /dev/hda';
  exit 1;
fi

DEVICE_SIZE=$(blockdev --getsize64 $DEVICE)
DEVICE_SIZE=$((DEVICE_SIZE / kGibiByte))
if [ $DEVICE_SIZE -lt $MINIMUM_DISK_SIZE ]; then
  # TODO(xushaohua): Read minimum size from conf file
  #error 'Error: At least 30G is required to install!';
  echo 'Error: At least 30G is required to install!';
fi

# Write bootloader info to conf.
installer_set "DI_BOOTLOADER" ${DEVICE}
installer_set "DI_ROOT_DISK" ${DEVICE}

if [ -d '/sys/firmware/efi' ]; then
  # First create a GPT partition table.
  parted -s $DEVICE mktable gpt || \
    error "Failed to create msdos partition on $DEVICE";

  if [ $DEVICE_SIZE -le $k15Gib ]; then
    make_efi

    parted -s $DEVICE mkpart primary ext4 512Mib 100% || \
      error "Failed to create partition ${DEVICE}2";
    flush_message
    mkfs.ext4 -F ${DEVICE}2 || \
      error "Failed to make ext4 filesystem on ${DEVICE}2";

    installer_set "DI_ROOT_PARTITION" ${DEVICE}2
    installer_set "DI_MOUNTPOINTS" "${DEVICE}1=/boot/efi"

  elif [ $DEVICE_SIZE -le $k60Gib ]; then
    make_efi

    # / on /dev/sda2
    START_SIZE=512
    END_SIZE=$((DEVICE_SIZE - k4Gib + START_SIZE))
    parted -s $DEVICE mkpart primary ext4 ${START_SIZE}Mib ${END_SIZE}Mib || \
      error "Failed to create linux-swap on ${DEVICE}2";
    flush_message
    mkfs.ext4 -F ${DEVICE}2 || \
      error "Failed to call mkswap ${DEVICE}2";

    # linux-swap on /dev/sda3
    START_SIZE=$END_SIZE
    parted -s $DEVICE mkpart primary linux-swap ${START_SIZE}Mib 100% || \
      error "Failed to create partition ${DEVICE}3";
    flush_message
    mkswap ${DEVICE}3 || \
      error "Failed to make linux-swap filesystem on ${DEVICE}3";

    installer_set "DI_ROOT_PARTITION" "${DEVICE}2"
    installer_set "DI_MOUNTPOINTS" "${DEVICE}1=/boot/efi;${DEVICE}3=swap"

  else
    make_efi

    # / on /dev/sda2
    START_SIZE=512
    END_SIZE=$((START_SIZE + k30Gib))
    parted -s $DEVICE mkpart primary ext4 ${START_SIZE}Mib ${END_SIZE}Mib || \
      error "Failed to create partition ${DEVICE}2";
    flush_message
    mkfs.ext4 -F ${DEVICE}2 || \
      error "Failed to make ext4 filesystem on ${DEVICE}2";

    START_SIZE=$END_SIZE
    END_SIZE=$((START_SIZE + k4Gib))
    # linux-swap on /dev/sda3
    parted -s $DEVICE mkpart primary linux-swap ${START_SIZE}Mib \
      ${END_SIZE}Mib || error "Failed to create linux-swap on ${DEVICE}3";
    flush_message
    mkswap ${DEVICE}3 || \
      error "Failed to call mkswap ${DEVICE}3";

    # /home on /dev/sda4
    START_SIZE=$END_SIZE
    parted -s $DEVICE mkpart primary ext4 ${START_SIZE}Mib 100% || \
      error "Failed to create partition ${DEVICE}4";
    flush_message
    mkfs.ext4 -F ${DEVICE}4 || \
      error "Failed to make ext4 filesystem on ${DEVICE}4";

    installer_set "DI_ROOT_PARTITION" "${DEVICE}2"
    installer_set "DI_MOUNTPOINTS" "${DEVICE}1=/boot/efi;${DEVICE}3=swap;${DEVICE}4=/home"
  fi
else
  # First create a msdos partition table.
  parted -s $DEVICE mktable msdos || \
    error "Failed to create msdos partition on $DEVICE";

  if [ $DEVICE_SIZE -le $k15Gib ]; then
    # / on /dev/sda1
    parted -s $DEVICE mkpart primary ext4 1Mib 100% || \
      error "Failed to create partition ${DEVICE}1";
    flush_message
    mark_device1_bootable
    mkfs.ext4 -F ${DEVICE}1 || \
      error "Failed to make ext4 filesystem on ${DEVICE}1";

    installer_set "DI_ROOT_PARTITION" "${DEVICE}1"

  elif [ $DEVICE_SIZE -le $k60Gib ]; then
    # / on /dev/sda1
    END_SIZE=$((DEVICE_SIZE - k4Gib))
    parted -s $DEVICE mkpart primary ext4 1Mib ${END_SIZE}Mib || \
      error "Failed to create partition ${DEVICE}1";
    flush_message
    mark_device1_bootable
    mkfs.ext4 -F ${DEVICE}1 || \
      error "Failed to make ext4 filesystem on ${DEVICE}1";

    # linux-swap on /dev/sda2
    START_SIZE=$END_SIZE
    parted -s $DEVICE mkpart primary linux-swap ${START_SIZE}Mib 100% || \
      error "Failed to create linux-swap on ${DEVICE}2";
    flush_message
    mkswap ${DEVICE}2 || \
      error "Failed to call mkswap ${DEVICE}2";

    installer_set "DI_ROOT_PARTITION" "${DEVICE}1"
    installer_set "DI_MOUNTPOINTS" "${DEVICE}2=swap"

  else
    # / on /dev/sda1
    START_SIZE=1
    END_SIZE=$k30Gib
    parted -s $DEVICE mkpart primary ext4 ${START_SIZE}Mib ${END_SIZE}Mib || \
      error "Failed to create partition ${DEVICE}1";
    flush_message
    mark_device1_bootable
    mkfs.ext4 -F ${DEVICE}1 || \
      error "Failed to make ext4 filesystem on ${DEVICE}1";

    # linux-swap on /dev/sda2
    START_SIZE=$END_SIZE
    END_SIZE=$((START_SIZE + k4Gib))
    parted -s $DEVICE mkpart primary linux-swap ${START_SIZE}Mib \
      ${END_SIZE}Mib || error "Failed to create linux-swap on ${DEVICE}2";
    flush_message
    mkswap ${DEVICE}2 || \
      error "Failed to call mkswap ${DEVICE}2";

    # /home on /dev/sda3
    START_SIZE=$END_SIZE
    parted -s $DEVICE mkpart primary ext4 ${START_SIZE}Mib 100% || \
      error "Failed to create partition ${DEVICE}3";
    flush_message
    mkfs.ext4 -F ${DEVICE}3 || \
      error "Failed to make ext4 filesystem on ${DEVICE}3";

    installer_set "DI_ROOT_PARTITION" "${DEVICE}1"
    installer_set "DI_MOUNTPOINTS" "${DEVICE}2=swap;${DEVICE}3=/home"
  fi
fi

# Commit to kernel.
partprobe

exit 0
