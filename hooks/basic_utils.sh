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

# This module defines basic utilities used in almost all scripts.

# Set environment
export LANG=C LC_ALL=C
export DEBIAN_FRONTEND="noninteractive"
export APT_OPTIONS='-y -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" --force-yes --no-install-recommends \
  --allow-unauthenticated'

# Absolute path to config file.
# Do not read from/write to this file, call installer_get/installer_set instead.
CONF_FILE=/etc/deepin-installer.conf

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
  which deepin-installer-settings 1>/dev/null || \
    exit "deepin-installer-settings not found!"
  deepin-installer-settings get "${CONF_FILE}" "${key}"
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key="$1"
  local value="$2"
  [ -z "${CONF_FILE}" ] && exit "CONF_FILE is not defined"
  which deepin-installer-settings 1>/dev/null || \
    exit "deepin-installer-settings not found!"
  deepin-installer-settings set "${CONF_FILE}" "${key}" "${value}"
}

# Check whether current platform is loongson or not.
is_loongson() {
  case $(uname -m) in
    loongson | mips*)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

# Check whether current platform is sw or not.
is_sw() {
  case $(uname -m) in
    sw*)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

# Check whether current platform is x86/x86_64 or not.
is_x86() {
  case $(uname -m) in
    i386 | i686 | amd64 | x86 | x86_64)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

# Check whether current platform is arm64 or not.
is_arm64() {
  case $(uname -m) in
    arm64 | aarch64)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}
