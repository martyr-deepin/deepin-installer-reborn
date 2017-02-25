#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Sync project repo and build.

rsync -ave ssh demo@ip:/tmp/deepin-installer-reborn/ installer \
  --exclude cmake-build-debug --exclude .git && \
  mkdir -p installer/build && \
  cd installer/build && \
  cmake -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make -j4 deepin-installer-reborn && \
  sudo install deepin-installer-reborn /usr/bin/ && \
  sudo cp -rf i18n/ /usr/share/deepin-installer-reborn/ && \
  sudo systemctl restart lightdm
