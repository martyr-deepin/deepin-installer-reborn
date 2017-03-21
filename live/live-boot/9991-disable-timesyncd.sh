#!/bin/sh
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Disable timesyncd service on live system startup.
# Or else it may write real time to BIOS.

[ -f /usr/lib/systemd/systemd-timesyncd ] && \
  rm -f /usr/lib/systemd/systemd-timesyncd

[ -f /usr/lib/systemd/system/systemd-timesyncd.service ] && \
  rm -f /usr/lib/systemd/system/systemd-timesyncd.service

[ -f /etc/systemd/system/sysinit.target.wants/systemd-timesyncd.service ] && \
  rm -f /etc/systemd/system/sysinit.target.wants/systemd-timesyncd.service
