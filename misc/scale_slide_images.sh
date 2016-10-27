#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Scale slide images to 960x640
cd ../resources/slide

for folder in $(ls); do
  if [ -d $folder ]; then
    cd $folder
    for image_file in $(ls); do
      convert $image_file -scale 960x640 /tmp/$image_file
      mv -vf /tmp/$image_file $image_file
    done
    cd ..
  fi
done

exit 0
