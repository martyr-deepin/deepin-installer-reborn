#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

error() {
  echo " "
  echo "Error: $*" >&2
  echo ""
  exit 1
}

kUsage="\
Usage $0 packets duration

packets -  number of captured packets before quit, 100 is recommended.
duration - seconds before quit, 10 is recommended.
"

if [ 2 -ne "$#" ]; then
  echo "${kUsage}" >&2
  exit 1
fi

packets=$1
duration=$2

# Capture beacon packets and print to stdout.
# Do not switch up "monitor mode" with airmon-ng, which might cause kernel
# crash in 4.4.0
tshark -c "${packets}" -a duration:${duration} -I \
  -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code

exit 0
