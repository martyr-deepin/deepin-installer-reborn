/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_SYSINFO_PROC_SWAPS_H
#define INSTALLER_SYSINFO_PROC_SWAPS_H

#include <QList>

namespace installer {

enum class SwapType {
  Partition,
  File,
};

struct SwapItem {
  QString filename;
  SwapType type;
  qint64 size;
  qint64 used;
  int priority;
};

typedef QList<SwapItem> SwapItemList;

// Parse /proc/swaps file.
SwapItemList ParseSwaps();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_PROC_SWAPS_H
