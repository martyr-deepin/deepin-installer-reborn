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

# Scale slide images to 960x640
cd ./resources/slide

for folder in $(ls); do
  if [ -d $folder ]; then
    cd $folder
    for image_file in $(ls); do
      PREFIX=${image_file%-*}
      EXT_NAME=${image_file#*.}
      echo "${PREFIX}.${EXT_NAME}"
      NEW_NAME="${PREFIX}.${EXT_NAME}"
      TMP_NEW_NAME="/tmp/${NEW_NAME}"
      mv "${image_file}" "${NEW_NAME}"
      convert "${NEW_NAME}" -scale 960x640 "${TMP_NEW_NAME}"
      mv -vf "${TMP_NEW_NAME}" "${NEW_NAME}"
    done
    cd ..
  fi
done

exit 0
