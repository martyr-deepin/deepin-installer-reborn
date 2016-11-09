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

misc_fs_umount() {
    msg "unmount misc filesystem"
    umount \
        "$1/tmp" \
        "$1/run" \
        "$1/dev/shm" \
        "$1/dev/pts" \
        "$1/dev" \
        "$1/sys" \
        "$1/proc"
}

misc_fs_mount() {
    msg "mount misc filesystem"
    mount -t proc proc "$1/proc" -o nosuid,noexec,nodev
    mount -t sysfs sys "$1/sys" -o nosuid,noexec,nodev
    mount -t devtmpfs udev "$1/dev" -o mode=0755,nosuid
    mount -t devpts devpts "$1/dev/pts" -o mode=0620,gid=5,nosuid,noexec
    mount -t tmpfs shm "$1/dev/shm" -o mode=1777,nosuid,nodev
    mount -t tmpfs run "$1/run" -o nosuid,nodev,mode=0755
    mount -t tmpfs tmp "$1/tmp" -o mode=1777,strictatime,nodev,nosuid
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

# message when starting a job
msg_job_start() {
    local mesg=$*
    echo " "
    echo " "
    echo ">> STARTING JOB ${mesg}"
}

msg_command_output() {
    local command=$*
    echo "Command: ${command}"
    echo "Output: "
    eval ${command} | sed 's/^/ /g'
    echo ""
}

# message when a job has been finished
msg_job_done() {
    local mesg=$*
    echo ">> JOB ${mesg} DONE"
    echo " "
    echo " "
}


# Check arguments
if [ $# -ne 1 ]; then
  error "Usage: $0 hook-file"
fi

hook_file=$1
in_chroot=$(grep 'in_chroot' ${hook_file})


# Check configuration
CONF_FILE=/etc/deepin-installer.conf
if [ ! -f ${CONF_FILE} ];then
    error "Configfile /etc/deepin-installer.conf does not exists."
fi
. ${CONF_FILE}

# Run hook file
if [ -z ${in_chroot} ]; then
  . ${hook_file}
  exit $?
else
  echo 'Run in chroot env'
  chroot /target ${hook_file}
  exit $?
fi
