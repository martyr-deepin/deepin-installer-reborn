#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Sync project repo and build.

rsync -ave ssh demo@ip:/tmp/deepin-installer/ installer \
  --exclude cmake-build-debug --exclude .git && \
  mkdir -p installer/build && \
  cd installer/build && \
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make -j4 deepin-installer && \
  sudo install deepin-installer /usr/bin/ && \
  sudo systemctl restart lightdm
