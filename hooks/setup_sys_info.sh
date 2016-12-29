#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Setup username, password, timezone and keyboard layout.

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

# Absolute path to config file.
# Do not read from/write to this file, call installer_get/installer_set instead.
_CONF_FILE=/etc/deepin-installer.conf


# Get value in conf file. Section name is ignored.
installer_get() {
  local key=$1
  deepin-installer-settings get ${_CONF_FILE} ${key}
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key=$1
  local value=$2
  deepin-installer-settings set ${_CONF_FILE} ${key} ${value}
}

setup_avatar() {
  DI_AVATAR=$(installer_get "DI_AVATAR")
  DI_USERNAME=$(installer_get "DI_USERNAME")

  USER_CONF=/var/lib/AccountService/users/${DI_USERNAME}
  AVATAR_FILE="file://${DI_AVATAR}"

  if [ -f "${DI_AVATAR}" ]; then
    deepin-installer-settings set "${USER_CONF}" "User" "Icon" "${AVATAR_FILE}"
  fi
}


setup_keyboard() {
  DI_KEYBOARD_LAYOUT=$(installer_get "DI_KEYBOARD_LAYOUT")
  DI_KEYBOARD_LAYOUT_VARIANT=$(installer_get "DI_KEYBOARD_LAYOUT_VARIANT")
  DI_KEYBOARD_MODEL=$(installer_get "DI_KEYBOARD_MODEL")
  DI_KEYBOARD_OPTIONS=$(installer_get "DI_KEYBOARD_OPTIONS")

  XKBMODEL=${DI_KEYBOARD_MODEL:-pc105}
  XKBLAYOUT=${DI_KEYBOARD_LAYOUT:-us}

  cat > /etc/default/keyboard << EOF
# Check /usr/share/doc/keyboard-configuration/README.Debian for
# documentation on what to do after having modified this file.

# The following variables describe your keyboard and can have the same
# values as the XkbModel, XkbLayout, XkbVariant and XkbOptions options
# in /etc/X11/xorg.conf.

XKBMODEL="${XKBMODEL}"
XKBLAYOUT="${XKBLAYOUT}"
XKBVARIANT="${DI_KEYBOARD_LAYOUT_VARIANT}"
XKBOPTIONS="${DI_KEYBOARD_OPTIONS}"

# If you don't want to use the XKB layout on the console, you can
# specify an alternative keymap.  Make sure it will be accessible
# before /usr is mounted.
# KMAP=/etc/console-setup/defkeymap.kmap.gz
EOF
}

setup_locale_timezone() {
  DI_LOCALE=$(installer_get "DI_LOCALE")
  DI_TIMEZONE=$(installer_get "DI_TIMEZONE")
  DI_LOCALE=${DI_LOCALE:-en_US}
  DI_TIMEZONE=${DI_TIMEZONE:-Etc/UTC}
  LOCALE=${DI_LOCALE%.*}

  # Alway generate en_US locale
  echo "Generating locale: en_US ${LOCALE}"
  sed -i "s/# \(en_US\.UTF-8.*$\)/\1/g" /etc/locale.gen

  if [ ! -z ${LOCALE} ];then
    sed -i "s/# \(${LOCALE}\.UTF-8.*$\)/\1/g" /etc/locale.gen
    cat > /etc/default/locale <<EOF
LANG=${LOCALE}.UTF-8
LANGUAGE=${LOCALE}
EOF
  else
    cat > /etc/default/locale <<EOF
LANG=en_US.UTF-8
LANGUAGE=en_US
EOF
  fi

  /usr/sbin/locale-gen

  echo "Check timezone ${DI_TIMEZONE}"
  if cat /usr/share/zoneinfo/zone.tab | grep -v '^#' | awk '{print $3}' | \
      grep -q "^${DI_TIMEZONE}$"; then
    echo "${DI_TIMEZONE} is available"
  else
    echo "Timezone is not available, Fall back to UTC"
    DI_TIMEZONE="Etc/UTC"
  fi

  echo "Set timezone to ${DI_TIMEZONE}"
  echo "${DI_TIMEZONE}" > /etc/timezone
  # Create /etc/localtime symbol links
  ln -nsf /usr/share/zoneinfo/${DI_TIMEZONE} /etc/localtime
  dpkg-reconfigure --frontend noninteractive tzdata

  # Set using local time or not
  DI_IS_LOCAL_TIME=$(installer_get "DI_IS_LOCAL_TIME")
  if [ x"${DI_IS_LOCAL_TIME}" = xtrue ]; then
    deepin-installer-adjtime 1
  fi

  # Check locale
  if ls /usr/share/i18n/locales | grep -q "\<${LOCALE}\>";then
    echo "${LOCALE} is available"
  else
    echo "${LOCALE} is not available, Fallback to en_US"
    unset LOCALE
  fi
}


setup_username_password() {
  DI_HOSTNAME=$(installer_get "DI_HOSTNAME")
  DI_PASSWORD=$(installer_get "DI_PASSWORD")
  DI_USERNAME=$(installer_get "DI_USERNAME")

  [ -z ${DI_USERNAME} ] && error "[ERROR] Variable DI_USERNAME not set. Aborting"
  [ -z ${DI_PASSWORD} ] && error "[ERROR] Variable DI_PASSWORD not set. Aborting."
  DI_HOSTNAME="${DI_HOSTNAME:-deepin}"

  # Reset password in settings file
  installer_set "DI_PASSWORD" ""

  if [ -x /bin/zsh ] ;then
    useradd -U -m --skel /etc/skel --shell /bin/zsh ${DI_USERNAME}
  else
    useradd -U -m --skel /etc/skel --shell /bin/bash ${DI_USERNAME}
  fi
  REAL_PASSWD=`echo ${DI_PASSWORD} | base64 -d -`
  echo ${DI_USERNAME}:"${REAL_PASSWD}" | chpasswd
  set +e
  gpasswd -a ${DI_USERNAME} sudo
  gpasswd -a ${DI_USERNAME} users
  gpasswd -a ${DI_USERNAME} wheel
  gpasswd -a ${DI_USERNAME} netdev
  gpasswd -a ${DI_USERNAME} storage
  gpasswd -a ${DI_USERNAME} lp
  gpasswd -a ${DI_USERNAME} scanner
  gpasswd -a ${DI_USERNAME} lpadmin
  gpasswd -a ${DI_USERNAME} network
  set -e

  echo "[INFO] Fix Home directory permission to ${DI_USERNAME}."
  chown -hR ${DI_USERNAME}:${DI_USERNAME} /home/${DI_USERNAME}

  echo "[INFO] Set Hostname to ${DI_HOSTNAME}"
  echo "${DI_HOSTNAME}" | tee /etc/hostname

  cat > /etc/hosts <<EOF
127.0.0.1	localhost
127.0.1.1   ${DI_HOSTNAME}

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
EOF

}

main() {
  setup_avatar
  setup_keyboard
  setup_locale_timezone
  setup_username_password
}

main
