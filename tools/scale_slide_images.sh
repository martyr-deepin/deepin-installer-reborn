#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

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
