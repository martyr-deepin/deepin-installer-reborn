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

#include "partman/os_prober.h"

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Cache output of `os-prober` command.
QString ReadOsProberOutput() {
  const QString cache_path("/tmp/deepin-installer-os-prober.conf");
  if (QFile::exists(cache_path)) {
    return ReadFile(cache_path);
  } else {
    if (!SpawnCmd("which", QStringList() << "os-prober")) {
      // os-prober not exist
      return QString();
    }

    // run os-prober once before ignore_uefi is created, so windows
    // in the efi partition can be found.
    QString output;
    if (!SpawnCmd("os-prober", {}, output)) {
      output.clear();
    }

    const QString partman_flag = "/var/lib/partman/ignore_uefi";
    if (!CreateParentDirs(partman_flag)) {
      qWarning() << "Failed to create parent folder of: " << partman_flag;
    }
    WriteTextFile(partman_flag, "deepin-installer");

    // run os-prober again after ignore_uefi created, so windows installed
    // in legacy mode will be found.
    QString second_time_output;
    if (SpawnCmd("os-prober", {}, second_time_output)) {
      output.append(second_time_output);
    }

    if (!output.isEmpty()) {
      WriteTextFile(cache_path, output);
      return output;
    } else {
      qCritical() << "os-prober failed";
      return QString();
    }
  }
}

}  // namespace

OsProberItems GetOsProberItems() {
  OsProberItems result;

  const QString output = ReadOsProberOutput();
  if (!output.isEmpty()) {
    for (const QString& line : output.split('\n')) {
      if (line.isEmpty()) {
        continue;
      }

      const QStringList items = line.split(':');
      Q_ASSERT(items.length() == 4);
      if (items.length() != 4) {
        continue;
      }

      // Removed version name from description.
      QString description = items.at(1);
      const int left_bracket_index = description.indexOf('(');
      if (left_bracket_index > -1) {
        description = description.left(left_bracket_index).trimmed();
      }
      const QString distro_name = items.at(2);

      OsType type;
      if (line.contains("linux", Qt::CaseInsensitive)) {
        type = OsType::Linux;
      } else if (line.contains("windows", Qt::CaseInsensitive)) {
        type = OsType::Windows;
      } else if (line.contains("mac", Qt::CaseInsensitive)) {
        // FIXME(xushaohua): Check macOS efi entry.
        type = OsType::Mac;
      } else {
        type = OsType::Unknown;
      }

      // Deal with EFI path, like "/dev/sda2@/EFI/Microsoft/Boot/bootmgfw.efi.
      const QString dev_path = items[0];
      const int index = dev_path.indexOf('@');
      const QString path = (index == -1) ? dev_path : dev_path.left(index);

      const OsProberItem prober_item = {path, description, distro_name, type};

      // Uniquify, since ReadOsProberOutput() contains duplicated entries.
      bool exists = false;
      for (const OsProberItem &item : result) {
        // TODO(hualet): overload the == operator.
        if (item.path == path \
            && item.type == type \
            && item.distro_name == distro_name \
            && item.description == description)
        {
          exists = true;
        }
      }
      if (!exists) {
        result.append(prober_item);
      }
    }
  }

  return result;
}

}  // namespace installer
