#!/bin/sh
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Replace \n with new line for deepin-installer.log file.

LOG_FILE=$1
if [ -f ${LOG_FILE} ]; then
  sed -e 's/\\r\\n/\n/g' $1 | sed -e 's/\\n/\n/g'
else
  echo "Usage: $0 deepin-installer.log"
  exit 1
fi
