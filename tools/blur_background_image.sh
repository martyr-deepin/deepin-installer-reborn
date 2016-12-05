#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Bluring image
for file in $*; do
  convert "$file" -blur 0x32 "blur-${file}"
done
