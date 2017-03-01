// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ARGS_PARSER_H
#define INSTALLER_UI_DELEGATES_ARGS_PARSER_H

#include <QStringList>

namespace installer {

// Parse argument list.
class ArgsParser {
 public:
  ArgsParser();

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

#endif  // INSTALLER_UI_DELEGATES_ARGS_PARSER_H
