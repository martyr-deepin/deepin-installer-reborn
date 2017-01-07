#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Setup username, password, timezone and keyboard layout.
# This script is used to setup system info after installation.

# Absolute path to config file.
# Do not read from/write to this file, call installer_get/installer_set instead.
CONF_FILE=/etc/deepin-installer.conf

# Absolute path to hooks folder.
HOOKS_DIR=/usr/share/deepin-installer-reborn

. "${HOOKS_DIR}/basic_utils.sh"

. "${HOOKS_DIR}/in_chroot/01_setup_locale_timezone.job"
. "${HOOKS_DIR}/in_chroot/03_configure_users.job"
. "${HOOKS_DIR}/in_chroot/04_setup_keyboard.job"
. "${HOOKS_DIR}/in_chroot/05_setup_avatar.job"

main() {
  setup_avatar && \
  setup_keyboard && \
  setup_locale_timezone && \
  setup_username_password

  # TODO(xushaohua): Clear packages.
}

main
