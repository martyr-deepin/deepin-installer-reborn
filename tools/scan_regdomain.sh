#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

usage() {
  echo "Usage $0 packets duration"
  echo ""
  echo "<packets> number of captured packets before quit, 100 is recommended"
  echo "<duration> seconds before quit, 20 is recommended"
}

if [ 2 -ne "$#" ]; then
  usage
  exit 1
fi

packets=$1
duration=$2

# Get appropriate wireless interface.
wifi_interface=$(airmon-ng  | grep '^[^mon][^Interface]' | cut -f 1)

if [ x$wifi_interface = x"" ]; then
  # No wireless interface found.
  exit 1
fi

# Enable monitor mode on $wifi_interface.
mon_interface=$(airmon-ng start $wifi_interface | grep 'enabled on ' | \
                cut -f 1 -d ')' | cut -f 5 -d ' ')

# Capture beacon packets and print to stdout.
#tshark -c $packets -a duration:$duration -I -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code 2>/dev/null
tshark -c $packets -a duration:$duration -I -f 'wlan[0] == 0x80' -Tfields -e wlan.sa -e wlan_mgt.country_info.code

# Disable monitor mode.
airmon-ng stop $mon_interface 1>/dev/null 2>&1

exit 0
