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

# Setup username, password, timezone and keyboard layout.
# This script is used to setup system info after installation.

# Absolute path to config file.
# Do not read/write this file directly, call installer_get and installer_set
# instead.
CONF_FILE=/etc/deepin-installer.conf

. ./basic_utils.sh

. ./in_chroot/09_generate_machine_id.job
. ./in_chroot/33_setup_lightdm_auto_login.job
. ./in_chroot/34_setup_livefs.job
. ./in_chroot/51_setup_keyboard.job
. ./in_chroot/52_setup_locale_timezone.job
. ./in_chroot/53_setup_user.job
. ./in_chroot/55_customize_user.job

# Check whether btrfs filesystem is used in machine.
detect_btrfs() {
  for i in $(lsblk -o FSTYPE | sed '/^$/d' | uniq); do
    [ "${i}" = "btrfs" ] && return 0
  done
  return 1
}

# Purge installer package
uninstall_installer() {
  # NOTE(xushaohua): Remove dependencies of installer by hand.
  # Until state of packages are correctly marked in ISO.
  if detect_btrfs; then
    apt-get -y purge deepin-installer tshark wireshark-common
  else
    apt-get -y purge deepin-installer btrfs-tools tshark wireshark-common
  fi
  apt-get -y autoremove --purge
}

# Replace lightdm.conf with lightdm.conf.real.
cleanup_lightdm_deepin_installer() {
  local CONF_FILE=/etc/lightdm/lightdm.conf
  local TEMP_CONF_FILE=/etc/lightdm/lightdm.conf.real
  if [ -f "${TEMP_CONF_FILE}" ]; then
    mv -f "${TEMP_CONF_FILE}" "${CONF_FILE}"
  fi
}

cleanup_first_boot() {
  local FILE=/etc/deepin-installer-first-boot
  [ -f "${FILE}" ] && rm -f "${FILE}"

  if [ -f /lib/systemd/system/deepin-installer.target ]; then
    # Restore default target of systemd
    systemctl set-default -f graphical.target
  else
    # See in_chroot/generate_reboot_setup_file.job for more info.
    cleanup_lightdm_deepin_installer
  fi
}

main() {
  [ -f "${CONF_FILE}" ] || error "deepin-installer.conf not found"
  cat "${CONF_FILE}"

  generate_machine_id
  setup_lightdm_auto_login
  setup_keyboard
  setup_locale_timezone
  setup_livefs

  # setup_username_password_hostname() will clear value of DI_PASSWORD
  setup_username_password_hostname
  customize_user

  sync
  cleanup_first_boot
  uninstall_installer
  sync
}

main
