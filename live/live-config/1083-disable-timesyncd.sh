#!/bin/sh
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Disable timesyncd service on live system startup.
# Or else it may write real time to BIOS.

for file in \
  /lib/systemd/systemd-timesyncd \
  /lib/systemd/system/systemd-timesyncd.service \
  /usr/lib/systemd/systemd-timesyncd \
  /usr/lib/systemd/system/systemd-timesyncd.service \
  /etc/systemd/system/sysinit.target.wants/systemd-timesyncd.service \
  ; do
  if [ -f "${file}" ]; then
    rm -f "${file}"
  fi
done



