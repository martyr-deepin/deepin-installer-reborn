#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

if [ $# -ne 1 ]; then
  echo "Usage: $0 hook-file"
  exit 1
fi

hook_file=$1
in_chroot=$(grep 'in_chroot' ${hook_file})

if [ -z ${in_chroot} ]; then
  . ${hook_file}
else
  echo 'Run in chroot env'
  chroot /target ${hook_file}
fi
