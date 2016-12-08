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
if [ $# -ne 1 ]; then
  error "Usage: $0 hook-file"
fi

SELF=$0
HOOK_FILE=$1
IN_CHROOT=$2

# Check configuration
CONF_FILE=/etc/deepin-installer.conf
if [ ! -f ${CONF_FILE} ];then
  error "Config file ${CONF_FILE} does not exists."
fi

# Get value in conf file. Section name is ignored.
installer_get() {
  local key=$1
  deepin-installer-conf-helper get ${CONF_FILE} ${key}
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key=$1
  local value=$2
  deepin-installer-conf-helper set ${CONF_FILE} ${key} ${value}
}

# Run hook file
case ${HOOK_FILE} in
  */in_chroot/*)
    if [ "x${IN_CHROOT}" = "xtrue" ]; then
      # Already in chroot env.
      # Host device is mounted at /target/deepinhost
      CONF_FILE="/deepinhost${CONF_FILE}"
      . "${HOOK_FILE}"
      exit $?
    else
      # Switch to chroot env.
      echo 'Run in chroot env'
      chroot /target "${SELF}" "${HOOK_FILE}"
      exit $?
    fi
    ;;
  *)
    # Run normal hooks.
    . "${HOOK_FILE}"
    exit $?
    ;;
esac
