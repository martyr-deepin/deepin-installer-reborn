// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// installer_conf_helper
// Get/set configuration in a ini file.
// Usage:
// * set ini-file section-name key value
// * set ini-file key value
// * get ini-file section-name key
// * get ini-file key

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
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

  CommandType command;
  if (pos_args.at(0) == kCommandGet) {
    command = CommandType::Get;
  } else if (pos_args.at(0) == kCommandSet) {
    command = CommandType::Set;
  } else {
    parser.showHelp(kExitErr);
  }

  const QString ini_file = pos_args.at(1);
  if ((command == CommandType::Get) && (!QFile::exists(ini_file))) {
    qCritical() << "File not found!" << ini_file;
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
    qDebug() << value;
  } else if (command == CommandType::Set) {
    if (section.isEmpty()) {
      settings.setValue(key, value);
    } else {
      settings.beginGroup(section);
      settings.setValue(key, value);
      settings.endGroup();
    }
  }

  return kExitOk;
}