// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_KERNEL_LIST_H_
#define INSTALLER_SERVICE_KERNEL_LIST_H_

#include <QStringList>

namespace installer {

struct KernelList {
  QString title;
  QStringList items;  // kernel list.
  int selected;  // Selected item. -1 means no item is selected.
};

// Read kernel list from json file.
KernelList GetKernelList();

}  // namespace installer

#endif  // INSTALLER_SERVICE_KERNEL_LIST_H_
