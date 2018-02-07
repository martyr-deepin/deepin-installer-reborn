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
