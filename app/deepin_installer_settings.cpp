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

// installer_conf_helper
// Get/set configuration in a ini file.
// Usage:
// * set ini-file section-name key value
// * set ini-file key value
// * get ini-file section-name key
// * get ini-file key

#include <stdio.h>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>
#include <QSettings>

namespace {

const char kAppVersion[] = "0.0.1";
const char kAppDesc[] = "Get/set configuration in a ini file.";

const int kExitErr = 1;
const int kExitOk = 0;

const char kCommandGet[] = "get";
const char kCommandSet[] = "set";

enum class CommandType {
  Get,
  Set,
  Invalid,
};

}  // namespace

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);
  app.setApplicationVersion(kAppVersion);

  QCommandLineParser parser;
  parser.setApplicationDescription(kAppDesc);
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("command", "Set or get value", "get/set");
  parser.addPositionalArgument("ini-file", "Absolute path to ini file");
  parser.addPositionalArgument("section",
                               "Section name in ini file",
                               "[section]");
  parser.addPositionalArgument("key", "Key name");
  parser.addPositionalArgument("value", "Value to be set", "[value]");

  if (!parser.parse(app.arguments())) {
    // Show help and exit.
    parser.showHelp(kExitErr);
  }

  const QStringList pos_args = parser.positionalArguments();

  if (pos_args.length() < 3 || pos_args.length() > 5) {
    parser.showHelp(kExitErr);
  }

  CommandType command = CommandType::Invalid;
  if (pos_args.at(0) == kCommandGet) {
    command = CommandType::Get;
  } else if (pos_args.at(0) == kCommandSet) {
    command = CommandType::Set;
  } else {
    parser.showHelp(kExitErr);
  }

  const QString ini_file = pos_args.at(1);
  if ((command == CommandType::Get) && (!QFile::exists(ini_file))) {
    fprintf(stderr, "File not found! %s\n", ini_file.toStdString().c_str());
    return kExitErr;
  }

  QString section;
  QString key;
  QString value;
  if (command == CommandType::Get) {
    if (pos_args.length() == 3) {
      key = pos_args.at(2);
    } else if (pos_args.length() == 4) {
      // Section name is optional.
      section = pos_args.at(2);
      key = pos_args.at(3);
    } else {
      parser.showHelp(kExitErr);
    }
  } else if (command == CommandType::Set) {
    if (pos_args.length() == 4) {
      key = pos_args.at(2);
      value = pos_args.at(3);
    } else if (pos_args.length() == 5) {
      section = pos_args.at(2);
      key = pos_args.at(3);
      value = pos_args.at(4);
    } else {
      parser.showHelp(kExitErr);
    }
  }

  QSettings settings(ini_file, QSettings::IniFormat);
  if (command == CommandType::Get) {
    if (section.isEmpty()) {
      value = settings.value(key).toString();
    } else {
      settings.beginGroup(section);
      value = settings.value(key).toString();
      settings.endGroup();
    }
    // Print value to stdout.
    fprintf(stdout, "%s", value.toStdString().c_str());

  } else if (command == CommandType::Set) {
    if (!section.isEmpty()) {
      settings.beginGroup(section);
    }
    if (value.contains(',')) {
      const QStringList list = value.split(',');
      settings.setValue(key, list);
    } else {
      settings.setValue(key, value);
    }
  }

  return kExitOk;
}
