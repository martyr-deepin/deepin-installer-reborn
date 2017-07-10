// Copyright (c) 2017 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/release_version.h"

#include <QSettings>

namespace installer {

ReleaseVersion GetReleaseVersioin() {
  ReleaseVersion version = {"", ""};
  QSettings settings("/etc/deepin-version", QSettings::IniFormat);
  settings.beginGroup("Release");
  if (settings.contains("Version")) {
    version.version = settings.value("Version").toString();
  }
  if (settings.contains("Type")) {
    version.type = settings.value("Type").toString();
  }
  settings.endGroup();

  return version;
}

}  // namespace installer