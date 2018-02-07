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

#ifndef INSTALLER_UI_DELEGATES_INSTALLER_ARGS_PARSER_H
#define INSTALLER_UI_DELEGATES_INSTALLER_ARGS_PARSER_H

#include <QStringList>

namespace installer {

// Parse argument list for deepin-installer.
// Do not use it in first-boot-setup.
class InstallerArgsParser {
 public:
  InstallerArgsParser();

  // Parse |args| list.
  bool parse(const QStringList& args);

  // Returns true if --auto-install is in args list.
  bool isAutoInstallSet() const;

  // Returns value of --conf-file option. Might be empty.
  QString getConfFile() const;

  // Returns value of --log-file option. Might be empty.
  QString getLogFile() const;

 private:
  bool auto_install_;
  QString conf_file_;
  QString log_file_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_INSTALLER_ARGS_PARSER_H
