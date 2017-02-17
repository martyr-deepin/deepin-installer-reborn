#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Automatically create disk partitions.
# Partition policy is defined in settings.ini.

kGibiByte=1048576

# Check device capacity of $1 is larger than required device size or not.
check_device_size() {
  local DEVICE=$1
  local DEVICE_SIZE
  DEVICE_SIZE=$(blockdev --getsize64 $DEVICE)
  DEVICE_SIZE=$((DEVICE_SIZE / kGibiByte))

  local MINIMUM_DISK_SIZE
  MINIMUM_DISK_SIZE=$(installer_get "partition_minimum_disk_space_required")

  if [ "${DEVICE_SIZE}" -lt "${MINIMUM_DISK_SIZE}" ]; then
    error "At least ${MINIMUM_DISK_SIZE} is required to install system!"
  fi
}

# Flush kernel message
flush_message() {
  udevadm settle --timeout=5
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

# The disk with largest storage capacity is used as system device.
get_max_capacity_device() {
  local DEVICE
  local MAX_CAPACITY=0
  lsblk -ndb -o NAME,SIZE 2>/dev/null | (while read NAME SIZE; do
    #echo $NAME:$SIZE
    #echo $DEVICE:$MAX_CAPACITY
    if [ $MAX_CAPACITY -lt $SIZE ]; then
      MAX_CAPACITY=$SIZE
      DEVICE=$NAME
    fi
  done && echo /dev/$DEVICE)
}

# Check boot mode is UEFI or not.
is_efi_mode() {
  test -d "/sys/firmware/efi"
}

# Read partition policy from settings.
get_partition_policy() {
  if is_efi_mode; then
     installer_get "partition_auto_part_uefi_policy"
  else
     installer_get "partition_auto_part_legacy_policy"
  fi
}

# Create new partition table.
new_partition_table() {
  local DEVICE=$1
  if is_efi_mode; then
    msg "Use UEFI mode"
    parted -s "${DEVICE}" mktable gpt || \
      error "Failed to create uefi partition on ${DEVICE}"
  else
    msg "Use legacy mode"
    parted -s ${DEVICE} mktable msdos || \
      error "Failed to create msdos partition on ${DEVICE}"
  fi
}

umount_devices() {
  # Umount all swap partitions.
  swapoff -a

  # Umount /target
  [ -d /target ] && umount -R /target
}

main() {
  # Based on the following process:
  #  * Umount devices
  #  * Get appropriate disk device.
  #  * Get boot mode.
  #  * Get partitioning policy.
  #  * Create new partition table.
  #  * Create partitions.
  #  * Notify kernel.

  umount_devices

  local DEVICE
  DEVICE=$(get_max_capacity_device)
  [ -e "${DEVICE}" ] || error "supported storage device not found"

  echo "Target device: ${DEVICE}"
  check_device_size "${DEVICE}"

  # Partitioning policy.
  local POLICY
  POLICY=$(get_partition_policy)
  echo "Partitioning policy: ${POLICY}"
  [ -z "${POLICY}" ] && error "Partitioning policy is empty!"

  new_partition_table "${DEVICE}"

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

    # Set boot flag.
    case "${PART_MP}" in
      /boot)
        # Set boot flag.
        if ! is_efi_mode; then
          installer_set "DI_BOOTLOADER" "${PART_PATH}"
          parted -s "${DEVICE}" set "${PART_NUM}" boot on || \
            error "Failed to set boot flag on ${PART_PATH}"
        fi
        ;;
      /)
        installer_set "DI_ROOT_PARTITION" "${PART_PATH}"
        # Set boot flag if /boot is not used in legacy mode.
        if ! is_efi_mode; then
          if ! grep -q "/boot:" "${POLICY} 2>/dev/null"; then
            installer_set "DI_BOOTLOADER" "${PART_PATH}"
            parted -s "${DEVICE}" set "${PART_NUM}" boot on || \
              error "Failed to set boot flag on ${PART_PATH}"
          fi
        fi
        ;;
      *)
        ;;
    esac

    flush_message

  done
  # Remove semicolon prefix.
  MP_LIST=$(echo "${MP_LIST}" | sed 's/^;//')
  installer_set "DI_MOUNTPOINTS" "${MP_LIST}"

  installer_set "DI_ROOT_DISK" "${DEVICE}"

  # Write boot method
  if is_efi_mode; then
    installer_set "DI_UEFI" "true"
  else
    installer_set "DI_UEFI" "false"
  fi
}

. ./basic_utils.sh

main
