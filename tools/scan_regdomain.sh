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

interfaceList=$(./airmon-ng)
if [ $? != 0 ]; then
  error "airmon-ng failed! ${interfaceList}"
fi

# Get the first availabel wireless device.
interface=$(echo "${interfaceList}" | cut -f 2 | grep '[^Interface]' | head -n1)
echo "interface: ${interface}"

if [ -z "${interface}" ]; then
  error "No interface availabel"
fi

# Capture beacon packets and print to stdout.
#tshark -c $packets -a duration:$duration -I -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code 2>/dev/null
tshark -c $packets -a duration:$duration -I -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code -i ${interface}

exit 0
