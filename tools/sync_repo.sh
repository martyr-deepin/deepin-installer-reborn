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

# Sync project repo and build.

rsync -ave ssh demo@ip:/tmp/deepin-installer/ installer \
  --exclude cmake-build-debug --exclude .git && \
  mkdir -p installer/build && \
  cd installer/build && \
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make -j4 deepin-installer && \
  sudo install deepin-installer /usr/bin/ && \
  sudo systemctl restart lightdm
