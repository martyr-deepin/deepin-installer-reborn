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
