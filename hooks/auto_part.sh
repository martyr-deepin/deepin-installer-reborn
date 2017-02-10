#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Automatically create disk partitions based on this policy:
# Partition policy is defined in settings.ini.

kGibiByte=1048576
k4Gib=4096
k15Gib=15360
k30Gib=30720
k60Gib=61440

# Minimum disk capacity required, 15Gib.
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

#  partprobe
}

# Format partition at $1 with filesystem $2
format_partition() {
  local PART_PATH=$1
  local PART_FS=$2

  case "${PART_FS}" in
    ext2 | ext3 | ext4)
      # Force mke2fs to create a filesystem.
      mkfs -t "${PART_FS}" -F "${PART_PATH}" || \
        error "Failed to create ${PART_FS} at ${PART_PATH}"
      ;;
    fat32)
      mkfs.vfat -F32 "${PART_PATH}" || \
        error "Failed to create ${PART_FS} at ${PART_PATH}"
      ;;
    fat16)
      mkfs.vfat -F16 "${PART_PATH}" || \
        error "Failed to create ${PART_FS} at ${PART_PATH}"
      ;;
    linux-swap)
      mkswap "${PART_PATH}" || error "Failed to create swap ${PART_PATH}"
      ;;
    *)
      mkfs "${PART_PATH}" || \
        error "Failed to create ${PART_FS} at ${PART_PATH}"
      ;;
  esac
}

# Check boot mode is UEFI or not.
is_efi_mode() {
  test -d "/sys/firmware/efi"
}

DEVICE=''

main() {
  # Based on the following process:
  #  * Get appropriate disk device.
  #  * Get boot mode.
  #  * Get partitioning policy.
  #  * Create new partition table.
  #  * Create partitions.
  #  * Notify kernel.

  # Umount all swap partitions.
#  swapoff -a || error "Failed to umount swap!"
  # TODO(xushaohua): Unmount other partitions.

  DEVICE=$(get_max_capacity_device)
  [ -z $DEVICE ] && error "no supported storage device found"

  DEVICE_SIZE=$(blockdev --getsize64 $DEVICE)
  DEVICE_SIZE=$((DEVICE_SIZE / kGibiByte))
  if [ $DEVICE_SIZE -lt $MINIMUM_DISK_SIZE ]; then
    # TODO(xushaohua): Read minimum size from conf file
    error "At least 30G is required to install!"
  fi

  # Write bootloader info to conf.
  installer_set "DI_ROOT_DISK" "${DEVICE}"

  echo "DEVICE: ${DEVICE}"

  # Partitioning policy.
  local POLICY
  if is_efi_mode; then
     POLICY=$(installer_get "partition_auto_part_uefi_policy")
  else
     POLICY=$(installer_get "partition_auto_part_legacy_policy")
  fi
  if [ -z "${POLICY}" ]; then
    error "Partitioning policy is empty!"
  fi

  # Create new partition table.
  if is_efi_mode; then
    msg "Use UEFI mode"
    parted -s "${DEVICE}" mktable gpt || \
      error "Failed to create msdos partition on ${DEVICE}"
  else
    msg "Use legacy mode"
    parted -s ${DEVICE} mktable msdos || \
      error "Failed to create msdos partition on ${DEVICE}"
  fi

  # Create new partitions.
  local PART_ARR PART_PATH PART_MP PART_FS PART_START PART_END
  local PART_NUM=0
  local MP_LIST
  local BOOTLOADER_NUM=0

  for PART in ${POLICY//;/ }; do
    # Increase partition number.
    PART_NUM=$((PART_NUM + 1))
    PART_PATH="${DEVICE}${PART_NUM}"

    PART_ARR=(${PART//:/ })
    PART_MP=${PART_ARR[0]}
    PART_FS=${PART_ARR[1]}
    PART_START=${PART_ARR[2]}
    PART_END=${PART_ARR[3]}

    echo "PART_ARR: ${PART_ARR}"
    echo "PART_MP: ${PART_MP}, ${PART_FS}, ${PART_START}, ${PART_END}"

    # Create new partition
    case "${PART_FS}" in
      efi)
        # Replace efi with fat32.
        echo "create efi partition"
        parted -s "${DEVICE}" mkpart primary fat32 \
          "${PART_START}" "${PART_END}" || \
           error "Failed to create new partition ${PART_MP}"
        ;;
      *)
        parted -s "${DEVICE}" mkpart primary "${PART_FS}" \
          "${PART_START}" "${PART_END}" || \
           error "Failed to create new partition ${PART_MP}"
        ;;
    esac

    flush_message

    # Create filesystem.
    case "${PART_FS}" in
      linux-swap)
        format_partition "${PART_PATH}" "${PART_FS}"
        MP_LIST="${MP_LIST};${PART_PATH}=swap"
        ;;
      efi)
        format_partition "${PART_PATH}" fat32
        MP_LIST="${MP_LIST};${PART_PATH}=${PART_MP}"

        installer_set "DI_BOOTLOADER" "${PART_PATH}"

        # Add boot flag
        parted -s "${DEVICE}" set "${PART_NUM}" esp on || \
          error "Failed to set esp flag on ${PART_PATH}"
        flush_message
        ;;
      *)
        format_partition "${PART_PATH}" "${PART_FS}"
        # TODO(xushaohua): remove semicolon;
        MP_LIST="${MP_LIST};${PART_PATH}=${PART_MP}"
        ;;
    esac

    flush_message

    case "${PART_MP}" in
      /boot)
        installer_set "DI_BOOTLOADER" "${PART_PATH}"

        # Set boot flag.
        if ! is_efi_mode; then
          parted -s "${DEVICE}" set "${PART_NUM}" boot on || \
            error "Failed to set boot flag on ${PART_PATH}"
        fi
        ;;
      /)
        installer_set "DI_ROOT_PARTITION" "${PART_PATH}"
        # Set boot flag if /boot is not used in legacy mode.
        if ! echo "${POLICY}" | grep -q "/boot:"; then
          installer_set "DI_BOOTLOADER" "${PART_PATH}"
          if ! is_efi_mode; then
            parted -s "${DEVICE}" set "${PART_NUM}" boot on || \
              error "Failed to set boot flag on ${PART_PATH}"
          fi
        fi
        ;;
      *)
        ;;
    esac

  done
  # Remove semicolon prefix.
  MP_LIST=$(echo "${MP_LIST}" | sed 's/^;//')
  installer_set "DI_MOUNTPOINTS" "${MP_LIST}"
}

. ./basic_utils.sh

main