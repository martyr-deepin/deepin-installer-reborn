#!/bin/bash
#
# Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Automatically create disk partitions.
# Partition policy is defined in settings.ini.

declare -i DEVICE_SIZE AVL_SIZE PART_NUM=0 LVM_NUM=0 LAST_END=1
declare DEVICE PART_POLICY MP_LIST PART_TYPE=primary LARGE=false EFI=false CRYPT=false LVM=false

# Check device capacity of $1 is larger than required device size or not.
check_device_size(){
  local minimum_disk_size=$(installer_get partition_minimum_disk_space_required)
  local large_disk_threshold=$(installer_get partition_full_disk_large_disk_threshold)
  DEVICE_SIZE=$(($(blockdev --getsize64 "$DEVICE") / 1024**2))

  if ((DEVICE_SIZE < minimum_disk_size * 1024)); then
    error "At least ${minimum_disk_size}Gib is required to install system!"
  elif ((DEVICE_SIZE > large_disk_threshold * 1024)); then
    declare -gr LARGE=true
  fi
}

# Check boot mode is UEFI or not.
check_efi_mode(){
  if [ -d "/sys/firmware/efi" ]; then
    declare -gr EFI=true
  fi
}

chech_use_crpyt(){
  DI_CRYPT_PASSWD=$(installer_get DI_CRYPT_PASSWD)
  if [ -n "$DI_CRYPT_PASSWD" ]; then
    declare -gr CRYPT=true
    installer_set DI_CRYPT_PASSWD 'NULL'
  fi
}

# Flush kernel message
flush_message(){
  udevadm settle --timeout=5
}

# Format partition at $1 with filesystem $2
format_part(){
  local part_mp="$1" part_fs="$2"

  yes |\
  case "$part_fs" in
    fat32)
      mkfs.vfat -F32 "$part_mp"
      ;;
    fat16)
      mkfs.vfat -F16 "$part_mp"
      ;;
    ntfs)
      mkfs.ntfs --fast "$part_mp"
      ;;
    linux-swap)
      mkswap "$part_mp"
      ;;
    *)
      mkfs -t "$part_fs" "$part_mp"
      ;;
  esac || error "Failed to create $part_fs at $part_mp"
}

# Read partition policy from settings.
get_part_policy(){
  local _policy_name="partition_full_disk"

  if $LARGE; then
    _policy_name+="_large"
  else
    _policy_name+="_small"
  fi

  if $EFI; then
    _policy_name+="_uefi"
  else
    _policy_name+="_legacy"
  fi

  if $CRYPT; then
    _policy_name+="_crypt"
  fi

  _policy_name+="_policy"
  declare -gr PART_POLICY="$(installer_get $_policy_name)"
}

get_max_capacity_device(){
  local name size max_device max_size=0
  while read name size; do
    if ((max_size <= size)); then
      max_size="$size"
      max_device="/edv/$name"
    fi
  done <<< "$(lsblk -ndb -o NAME,SIZE 2>/dev/null)"
  DEVICE="$max_device"
}

# Create new partition table.
new_part_table(){
  local _part_table=msdos
  if $EFI; then
    _part_table=gpt
  fi

  parted -s "$DEVICE" mktable "$_part_table" || \
    error "Failed to create $_part_table partition on $DEVICE"
}

umount_devices(){
  # Umount all swap partitions.
  swapoff -a

  # Umount /target
  [ -d /target ] && umount -R /target
}

# Create partition.
create_part(){
  local part="$1"
  local swap_size part_path part_mp part_fs _part_fs part_start part_end mapper_name

  let PART_NUM++
  echo "============PART_NUM: $PART_NUM============"

  # Create extended partition.
  if ! ( $EFI || $LVM ) && [ "$PART_NUM" = 4 ]; then
    echo "Create extended partition!"
    parted -s "$DEVICE" mkpart extended "${LAST_END}Mib" 100%
    let PART_NUM++
    echo "============PART_NUM: $PART_NUM============"
    PART_TYPE=logical
  fi

  # Get partition info.
  [[ "$part" =~ ^(.*):(.*):(.*):(.*)$ ]] || error "bad partition info!"
  part_mp="${BASH_REMATCH[1]}"
  part_fs="${BASH_REMATCH[2]}"
  part_size="${BASH_REMATCH[4]}"
  if ! $LVM; then
    part_start="${BASH_REMATCH[3]}"
    if [ -z "$part_start" ]; then
      if [ "$PART_TYPE" = "logical" ]; then
        part_start=$((LAST_END + 1))
      else
        part_start="$LAST_END"
      fi
    fi
    AVL_SIZE=$((DEVICE_SIZE - 1 - part_start))
  else
    [[ "$(vgs -ovg_free --readonly --units m vg0)" =~ ([0-9]+) ]] &&\
      AVL_SIZE="${BASH_REMATCH[1]}"
  fi

  if [[ "$part_size" =~ %$ ]]; then
    part_size=$((AVL_SIZE * ${part_size%\%} / 100))
  elif [ "$part_size" = swap-size ] && [[ "$(free -g)" =~ Mem:\ +([0-9]+) ]]; then
    swap_size="${BASH_REMATCH[1]}"
    let swap_size+=$(echo "sqrt($swap_size)"|bc)
    if ((swap_size > 0)); then
      part_size=$((swap_size * 1024))
    else
      part_size=1024
    fi
  fi

  # Check root partition size
  if [ "$part_mp" = '/' ] &&\
  [[ "$(installer_get partition_full_disk_large_root_part_range)" =~ ^([0-9]+):([0-9]+)$ ]]; then
    local root_min=$((BASH_REMATCH[1] * 1024)) root_max=$((BASH_REMATCH[2] * 1024))
    part_size=$((part_size < root_min ? root_min : part_size))
    part_size=$((part_size > root_min ? root_max : part_size))
  fi

  echo "part: $part_mp, $part_fs, $part_start, $part_size"

  # Create new partition
  if ! $LVM; then
    case "$part_fs" in
      efi)
        _part_fs=fat32;;
      crypto_luks)
        _part_fs='';;
      *)
        printf -v _part_fs '%q' "$part_fs";;
    esac
    part_end=$((part_start + part_size))
    if parted -s "$DEVICE" mkpart "$PART_TYPE" $_part_fs "${part_start}Mib" "${part_end}Mib"; then
      if [[ "$DEVICE" =~ [0-9]$ ]]; then
        part_path="${DEVICE}p${PART_NUM}"
      else
        part_path="${DEVICE}${PART_NUM}"
      fi
      LAST_END="$part_end"
    else
      error "Failed to create new partition $part_mp"
    fi
  else
    let LVM_NUM++
    lvcreate -nlv$LVM_NUM -L$part_size vg0
    part_path="/dev/vg0/lv$LVM_NUM"
  fi

  flush_message

  # Create filesystem.
  case "$part_fs" in
    efi)
      format_part "$part_path" fat32

      # No need to append EFI partition to mount point list.
      # MP_LIST="${MP_LIST+$MP_LIST;}$part_path=$part_mp"

      installer_set "DI_BOOTLOADER" "$part_path"

      # Add boot flag
      parted -s "$DEVICE" set "$PART_NUM" esp on || \
        error "Failed to set esp flag on $part_path"
      ;;
    crypto_luks)
      mapper_name="$part_mp"
      part_mp="/dev/mapper/$mapper_name"
      installer_set DI_CRYPT_ROOT "true"
      installer_set DI_CRYPT_PARTITION "$part_path"
      installer_set DI_CRYPT_TARGET "$mapper_name"
      {
        echo -n "$DI_CRYPT_PASSWD" | cryptsetup -v luksFormat "$part_path"
        echo -n "$DI_CRYPT_PASSWD" | cryptsetup open "$part_path" "$mapper_name"
        unset DI_CRYPT_PASSWD
      } || error "Failed to create luks partition!"
      pvcreate "$part_mp" -ffy
      vgcreate vg0 "$part_mp"
      declare -gr LVM=true
      ;;
    *)
      format_part "$part_path" "$part_fs"
      if [ -n "$part_mp" ]; then
        MP_LIST="${MP_LIST+$MP_LIST;}$part_path=$part_mp"
      fi
      ;;
  esac

  flush_message

  # Set boot flag.
  case "$part_mp" in
    /boot)
      # Set boot flag.
      ! $EFI && parted -s "$DEVICE" set "$PART_NUM" boot on || \
        error "Failed to set boot flag on $part_path"
      ;;
    /)
      installer_set "DI_ROOT_PARTITION" "$part_path"
      # Set boot flag if /boot is not used in legacy mode.
      if ! $EFI && ! $LVM && ! [[ "$PART_POLICY" =~ "/boot:" ]]; then
        parted -s "$DEVICE" set "$PART_NUM" boot on || \
          error "Failed to set boot flag on $part_path"
      fi
      ;;
  esac

  flush_message
}

main(){
  # Based on the following process:
  #  * Umount devices
  #  * Get boot mode.
  #  * Get partitioning policy.
  #  * Create new partition table.
  #  * Create partitions.
  #  * Notify kernel.

  umount_devices

  DEVICE="$(installer_get DI_FULLDISK_DEVICE)"
  if [ "$DEVICE" = auto_max ]; then
    get_max_capacity_device
  fi
  [ -b "$DEVICE" ] || error "Device not found!"
  echo "Target device: $DEVICE"

  check_device_size
  chech_use_crpyt
  check_efi_mode

  # Partitioning policy.
  get_part_policy
  echo "Partitioning policy: $PART_POLICY"
  [ -n "$PART_POLICY" ] || error "Partitioning policy is empty!"

  new_part_table "$DEVICE"

  for part in ${PART_POLICY//;/ }; do
    create_part "$part"
  done

  installer_set DI_MOUNTPOINTS "$MP_LIST"
  installer_set DI_ROOT_DISK "$DEVICE"

  # Write boot method
  if $EFI; then
    installer_set DI_UEFI "true"
  else
    installer_set DI_BOOTLOADER "$DEVICE"
    installer_set DI_UEFI "false"
  fi
}

. ./basic_utils.sh

DI_CUSTOM_PARTITION_SCRIPT=$(installer_get "DI_CUSTOM_PARTITION_SCRIPT")
if [ -f "${DI_CUSTOM_PARTITION_SCRIPT}" ]; then
  echo "call custom partition script" ${DI_CUSTOM_PARTITION_SCRIPT}
  bash ${DI_CUSTOM_PARTITION_SCRIPT}
else
  main
fi
