#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Bluring image
for file in $*; do
  output_name=blur-$(basename "${file}")
  convert "${file}" -resize 25% -brightness-contrast -10 \
    -modulate 100,120,100 -blur 0x16 -resize 400% "${output_name}"
done
