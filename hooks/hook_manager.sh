#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Set environment
export LC_ALL=C
export DEBIAN_FRONTEND="noninteractive"
export APT_OPTIONS="-y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" --force-yes --no-install-recommends --allow-unauthenticated"

# Print error message and exit
error() {
  local msg=$*
  echo " "
  echo "!! Error: ${msg}" >&2
  echo " "
  exit 1
}

warn() {
  local msg=$*
  echo "Warn: ${msg}" >&2
}

warn_exit() {
  local msg=$*
  echo "Warn: ${msg}" >&2
  exit 0
}

# standard message
msg() {
  local mesg=$*
  echo ":: BACKEND : ${mesg}"
}

debug() {
  local mesg=$*
  echo " - ${mesg}"
}

# Check arguments
if [ $# -lt 1 ]; then
  error "Usage: $0 hook-file"
fi

# Absolue path of hook_manager.sh in chroot env.
# This path is defined in service/backend/hooks_pack.cpp.
SELF=/tmp/installer-reborn/hook_manager.sh
HOOK_FILE=$1
IN_CHROOT=$2

# Check configuration
CONF_FILE=/etc/deepin-installer.conf

# Get value in conf file. Section name is ignored.
installer_get() {
  local key=$1
  deepin-installer-settings get ${CONF_FILE} ${key}
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key=$1
  local value=$2
  deepin-installer-settings set ${CONF_FILE} ${key} ${value}
}

# Run hook file
case ${HOOK_FILE} in
  */in_chroot/*)
    if [ "x${IN_CHROOT}" = "xtrue" ]; then
      # Already in chroot env.
      # Host device is mounted at /target/deepinhost
      CONF_FILE="/deepinhost${CONF_FILE}"
      if [ ! -f ${CONF_FILE} ];then
        error "Config file ${CONF_FILE} does not exists."
      fi
      . "${HOOK_FILE}"
      exit $?
    else
      # Switch to chroot env.
      echo 'Run in chroot env'
      chroot /target "${SELF}" "${HOOK_FILE}" 'true'
      exit $?
    fi
    ;;
  *)
    # Run normal hooks.
    if [ ! -f ${CONF_FILE} ];then
      error "Config file ${CONF_FILE} does not exists."
    fi
    . "${HOOK_FILE}"
    exit $?
    ;;
esac
