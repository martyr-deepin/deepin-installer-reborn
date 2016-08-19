#!/bin/bash

# Automatically create disk partitions.
#  * Create label of /dev/sda to MS-DOS.
#  * Create /dev/sda1 with 4G as linux-swap.
#  * Create /dev/sda2 with 15G as / with ext4 filesystem type.
#  * Use the remaining space as /home with ext4 filesystem type.

# Print error message and exit current context.
error_exit() {
  echo $*
  exit 1;
}

# Path to installer configuration file.
CONF_FILE='/etc/deepin-installer.conf'

# Minimum disk capacity required, 30G.
MINIMUM_DISK_SIZE=30000

# 4G space for linux-swap.
SWAP_SIZE=4096

# 15G space for /
ROOT_SIZE=15360

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

# Umount all swap partitions.
swapoff -a || error_exit "Failed to umount swap!"

DEVICE=$(get_max_capacity_device)
if [ -z $DEVICE ]; then
  echo 'Error: no supported storage device found!';
  echo 'There shall be a IDE or SATA disk available at /dev/sda or /dev/hda';
  exit 1;
fi

DEVICE_SIZE=$(blockdev --getsize64 $DEVICE)
DEVICE_SIZE=$((DEVICE_SIZE / 1048576))
if [ $DEVICE_SIZE -lt $MINIMUM_DISK_SIZE ]; then
  error_exit 'Error: At least 30G is required to install!';
fi

REMAINING_SIZE=$((DEVICE_SIZE - SWAP_SIZE))

# Write bootloader info to conf.
sed -i "s|DI_BOOTLOADER=.*$|DI_BOOTLOADER=\"$DEVICE\"|" $CONF_FILE
sed -i "s|DI_ROOT_DISK=.*$|DI_ROOT_DISK=\"$DEVICE\"|" $CONF_FILE

# First create a msdos partition table.
parted -s $DEVICE mktable msdos || \
  error_exit "Failed to create msdos partition on $DEVICE";

# Then create a swap partition with 4G.
parted -s $DEVICE mkpart primary linux-swap 1M ${SWAP_SIZE}M || \
  error_exit "Failed to create linux-swap on ${DEVICE}1";
mkswap ${DEVICE}1 || \
  error_exit "Failed to call mkswap ${DEVICE}1";

# And then create an ext4 partition with 20G capacity to mount /var/log
parted -s $DEVICE mkpart primary ext4 ${SWAP_SIZE}M ${ROOT_SIZE}M || \
  error_exit "Failed to create partition ${DEVICE}2";
mkfs.ext4 -F ${DEVICE}2 || \
  error_exit "Failed to make ext4 filesystem on ${DEVICE}2";
sed -i "s|DI_ROOT_PARTITION=.*$|DI_ROOT_PARTITION=\"${DEVICE}2\"|" $CONF_FILE

# All remaining space is used as backup partition.
parted -s $DEVICE mkpart primary ext4 ${LOG_SIZE}M 100% || \
  error_exit "Failed to create partition ${DEVICE}4";
mkfs.ext4 -F ${DEVICE}3 || \
  error_exit "Failed to make ext4 filesystem on ${DEVICE}3";

# Commit to kernel.
partprobe

exit 0
