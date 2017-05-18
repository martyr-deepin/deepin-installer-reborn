#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Generate image with blur effect.
# If no parameter is specified, update installer background image in system.

# Bluring image from $1 to $2
blur_image() {
  local SRC=$1
  local DST=$2
  local TMP=/tmp/installer-$(date +%s)-$(basename "${SRC}")
  if [ -f "${SRC}" ]; then
    convert "${SRC}" -resize 25% -brightness-contrast -10 \
      -modulate 100,120,100 -blur 0x16 -resize 400% "${TMP}" && \
      mv -f "${TMP}" "${DST}"
  else
    echo "${SRC} not found" >&2
    return 1
  fi
}

readonly ARGS_NUM=$#
if [ "${ARGS_NUM}" -eq 0 ]; then
  blur_image /usr/share/backgrounds/default_background.jpg \
    /usr/share/deepin-installer/resources/default_wallpaper.jpg
elif [ "${ARGS_NUM}" -eq 2 ]; then
  blur_image "$1" "$2"
else
  echo "Usage: $0 src-file dst-file" >&2
  exit 1
fi
