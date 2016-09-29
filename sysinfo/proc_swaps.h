// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
