#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Setup username, password, timezone and keyboard layout.
# This script is used to setup system info after installation.

# Absolute path to config file.
# Do not read/write this file directly, call installer_get and installer_set
# instead.
CONF_FILE=/etc/deepin-installer.conf

. ./basic_utils.sh

. ./in_chroot/01_setup_locale_timezone.job
. ./in_chroot/03_configure_users.job
. ./in_chroot/04_setup_keyboard.job
. ./in_chroot/05_setup_avatar.job
. ./in_chroot/11_generate_machine_id.job

# Check whether btrfs filesystem is used in machine.
detect_btrfs() {
  for i in $(lsblk -o FSTYPE | sed '/^$/d' | uniq); do
    [ "${i}" = "btrfs" ] && return 0
  done
  return 1
}

# Purge installer package
uninstall_installer() {
  if detect_btrfs; then
    apt-get -y purge deepin-installer
  else
    apt-get -y purge deepin-installer btrfs-tools
  fi
  apt-get -y autoremove --purge
}

cleanup_first_boot() {
  local FILE=/etc/deepin-installer-first-boot
  [ -f "${FILE}" ] && rm -f "${FILE}"

  # Restore default target of systemd
  systemctl set-default -f graphical.target
}

main() {
  [ -f "${CONF_FILE}" ] || error "deepin-installer.conf not found"
  cat "${CONF_FILE}"

  setup_avatar
  setup_keyboard
  setup_locale_timezone
  setup_username_password
  generate_machine_id

  cleanup_first_boot

  uninstall_installer
}

main
