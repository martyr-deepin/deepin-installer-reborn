// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_SYSINFO_RELEASE_VERSION_H
#define DEEPIN_INSTALLER_SYSINFO_RELEASE_VERSION_H

#include <QString>

namespace installer {

struct ReleaseVersion {
  QString version;
  QString type;
};

ReleaseVersion GetReleaseVersioin();

}  // namespace installer

#endif  // DEEPIN_INSTALLER_SYSINFO_RELEASE_VERSION_H
