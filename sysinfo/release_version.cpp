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