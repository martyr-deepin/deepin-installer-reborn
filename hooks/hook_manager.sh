#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Set environment
export LC_ALL=C
export DEBIAN_FRONTEND="noninteractive"
export APT_OPTIONS="-y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" --force-yes --no-install-recommends --allow-unauthenticated"

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

hook_file=$1

# Check configuration
CONF_FILE=/etc/deepin-installer.conf
if [ ! -f ${CONF_FILE} ];then
  error "Config file /etc/deepin-installer.conf does not exists."
fi
. ${CONF_FILE}

# Run hook file
case ${hook_file} in
  */in_chroot/*)
    echo 'Run in chroot env'
    # Host device is mounted at /target/deepinhost
    chroot /target /deepinhost/${hook_file}
    exit $?
    ;;
  *)
    . ${hook_file}
    exit $?
    ;;
esac
