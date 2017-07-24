#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Copy oem folder into ISO image file.

error() {
  echo "ERROR: $* " >&2
  exit 1
}

installDependencies() {
  echo '[installDependencies]'
  which xorriso || sudo apt install -y xorriso
}

# Extract contents of original ISO.
extractISO() {
  echo '[extractISO]'
  local kISOMountPointDir=$HOME/.cache/deepin/deepin-installer/$(date +%s).mp
  mkdir -pv "${kISOMountPointDir}"
  mkdir -pv "${kSourceISODir}"

  sudo mount "${kSourceISOFile}" "${kISOMountPointDir}" && \
    echo "Copy content in iso file..."
    rsync -av --delete "${kISOMountPointDir}/" "${kSourceISODir}" && \
    sudo umount "${kISOMountPointDir}" && \
    rmdir -v "${kISOMountPointDir}"
}

copyOemFolder() {
  echo '[copyOemFolder]'
  rsync -av --delete "${kOemDir}/" "${kSourceISODir}/oem"
}

# Generate new ISO
generateISO() {
  echo '[generateISO]'
#  xorriso -as mkisofs -D -r -V "$kDistroName $kVersion (${kArch})" \
  xorriso -as mkisofs -D -r \
    -cache-inodes -J -l -b isolinux/isolinux.bin -c isolinux/boot.cat \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    -input-charset utf-8 \
    -o "${kTargetISOFile}" "${kSourceISODir}" && \
    rm -rf "${kSourceISODir}"
}

if [ $# != 3 ]; then
  error "Usage: $0 input-iso oem-folder output-iso"
fi

kSourceISOFile=$1
kOemDir=$2
kTargetISOFile=$3
kSourceISODir=$HOME/.cache/deepin/deepin-installer/$(date +%s).dir

installDependencies || error "Failed to install dependencies"
extractISO || error "Failed to extract base iso"
copyOemFolder || error "Failed to copy oem folder"
generateISO || error "Failed to generate new iso"