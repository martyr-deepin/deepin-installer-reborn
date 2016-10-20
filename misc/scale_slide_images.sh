#!/bin/bash

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
