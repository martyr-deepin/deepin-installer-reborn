#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Entry point for hook scripts.
# This script setups variables and function used in hook script.
# Also handles chroot environment.

# Absolute path to config file.
# Do not read from/write to this file, call installer_get/installer_set instead.
CONF_FILE=/etc/deepin-installer.conf

. ./basic_utils.sh

# Check arguments
if [ $# -lt 1 ]; then
  error "Usage: $0 hook-file"
fi

# Absolute path of hook_manager.sh in chroot env.
# This path is defined in service/backend/hooks_pack.cpp.
_SELF=/tmp/installer-reborn/hook_manager.sh
_HOOK_FILE=$1
_IN_CHROOT=$2

# Defines absolute path to oem folder.
# /tmp/oem is reserved for debug.
OEM_DIR=/media/cdrom/oem
if [ -d "/tmp/oem" ]; then
  OEM_DIR=/tmp/oem
fi
# Mark $OEM_DIR as readonly constant.
readonly OEM_DIR

# Run hook file
case ${_HOOK_FILE} in
  */in_chroot/*)
    if [ "x${_IN_CHROOT}" = "xtrue" ]; then
      # Already in chroot env.
      # Host device is mounted at /target/deepinhost
      CONF_FILE="/deepinhost${CONF_FILE}"
      if [ ! -f "${CONF_FILE}" ];then
        error "Config file ${CONF_FILE} does not exists."
      fi
      . "${_HOOK_FILE}"
      exit $?
    else
      # Switch to chroot env.
      msg "Run in chroot env"
      chroot /target "${_SELF}" "${_HOOK_FILE}" 'true'
      exit $?
    fi
    ;;
  *)
    # Run normal hooks.
    if [ ! -f "${CONF_FILE}" ];then
      error "Config file ${CONF_FILE} does not exists."
    fi
    . "${_HOOK_FILE}"
    exit $?
    ;;
esac
