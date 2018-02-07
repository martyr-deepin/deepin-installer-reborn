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