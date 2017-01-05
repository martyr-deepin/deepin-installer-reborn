#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# This module defines basic utilities used in almost all scripts.

# Set environment
export LANG=C LC_ALL=C
export DEBIAN_FRONTEND="noninteractive"
export APT_OPTIONS='-y -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" --force-yes --no-install-recommends \
  --allow-unauthenticated'

# Print error message and exit
error() {
  local msg="$@"
  echo " "
  echo "!! Error: ${msg}" >&2
  echo " "
  exit 1
}

warn() {
  local msg="$@"
  echo "Warn: ${msg}" >&2
}

warn_exit() {
  local msg="$@"
  echo "Warn: ${msg}" >&2
  exit 0
}

# standard message
msg() {
  local msg="$@"
  echo "Info: ${msg}"
}

debug() {
  local msg="$@"
  echo "Debug: ${msg}"
}

# Get value in conf file. Section name is ignored.
# NOTE(xushaohua): Global variant or environment $CONF_FILE must not be empty.
installer_get() {
  local key="$1"
  [ -z "${CONF_FILE}" ] && exit "CONF_FILE is not defined"
  deepin-installer-settings get "${CONF_FILE}" "${key}"
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key="$1"
  local value="$2"
  [ -z "${CONF_FILE}" ] && exit "CONF_FILE is not defined"
  deepin-installer-settings set "${CONF_FILE}" "${key}" "${value}"
}