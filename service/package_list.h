// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_PACKAGE_LIST_H_
#define INSTALLER_SERVICE_PACKAGE_LIST_H_

#include <QList>
#include <QString>

namespace installer {

struct PackageList {
  QString title;
  QStringList items;
  QList<int> selected;
};

// Read package list from json file.
PackageList GetPackageList();

}  // namespace installer

#endif  // INSTALLER_SERVICE_PACKAGE_LIST_H_
