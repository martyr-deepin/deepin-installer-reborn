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

# Entry point for hook scripts.
# This script setups variables and function used in hook script.
# Also handles chroot environment.

# Folder path of hooks.
HOOKS_DIR=/tmp/installer

if [ ! -d ${HOOKS_DIR} ];then
  HOOKS_DIR=/usr/share/deepin-installer/hooks
fi

. "${HOOKS_DIR}/basic_utils.sh"

# Check arguments
if [ $# -lt 1 ]; then
  error "Usage: $0 hook-file"
fi

# Absolute path of hook_manager.sh in chroot env.
# This path is defined in service/backend/hooks_pack.cpp.
_SELF="${HOOKS_DIR}/hook_manager.sh"
_HOOK_FILE=$1
_IN_CHROOT=$2

# Defines absolute path to oem folder.
# /tmp/oem is reserved for debug.
if [ -d /tmp/oem ]; then
  # Debug mode
  OEM_DIR=/tmp/oem
elif [ -d /media/cdrom/oem ]; then
  # chroot mode
  OEM_DIR=/media/cdrom/oem
elif [ -d /lib/live/mount/medium/oem ]; then
  # chroot mode
  OEM_DIR=/lib/live/mount/medium/oem
elif [ -d /media/apt/oem ]; then
  # chroot mode
  # FIXME: maybe apt will change mount point
  # /media/cdrom => /media/apt
  # hook script invalid
  OEM_DIR=/media/apt/oem
fi

# Mark $OEM_DIR as readonly constant.
readonly OEM_DIR

# Run hook file
case ${_HOOK_FILE} in
  */in_chroot/*)
    if [ "x${_IN_CHROOT}" = "xtrue" ]; then
      if [ ! -f "${CONF_FILE}" ]; then
        error "Config file ${CONF_FILE} does not exists."
      fi
      . "${_HOOK_FILE}"
      exit $?
    else
      # Switch to chroot env.
      chroot /target "${_SELF}" "${_HOOK_FILE}" 'true'
      exit $?
    fi
    ;;
  *)
    # Run normal hooks.
    if [ ! -f "${CONF_FILE}" ]; then
      error "Config file ${CONF_FILE} does not exists."
    fi
    . "${_HOOK_FILE}"
    exit $?
    ;;
esac
