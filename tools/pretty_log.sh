#!/bin/sh
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

# Replace \n with new line for deepin-installer.log file.

LOG_FILE=$1
if [ -f ${LOG_FILE} ]; then
  sed -e 's/\\r\\n/\n/g' $1 | sed -e 's/\\n/\n/g'
else
  echo "Usage: $0 deepin-installer.log"
  exit 1
fi
