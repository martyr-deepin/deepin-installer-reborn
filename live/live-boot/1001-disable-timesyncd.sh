#!/bin/sh
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Disable timesyncd service on live system startup.
# Or else it may write real time to BIOS.

readonly TIME_SERVICE=/etc/systemd/system/sysinit.target.wants/systemd-timesyncd.service
if [ -f "${TIME_SERVICE}" ]; then
  rm "${TIME_SERVICE}"
fi