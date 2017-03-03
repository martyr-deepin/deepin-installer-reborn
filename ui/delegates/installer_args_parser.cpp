// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/installer_args_parser.h"

#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineOption>
#include <QCommandLineParser>

namespace installer {

InstallerArgsParser::InstallerArgsParser()
    : auto_install_(false),
      conf_file_(""),
      log_file_("") {
}

bool InstallerArgsParser::parse(const QStringList& args) {
  // Reset options.
  auto_install_ = false;
  conf_file_ = "";
  log_file_ = "";

  // Add pre-defined options.
  QCommandLineParser parser;
  const QCommandLineOption conf_file_option(
      "conf","Read config from <file>", "file", "");
  parser.addOption(conf_file_option);
  const QCommandLineOption log_file_option(
      "log", "Write log to <file>", "file", "");
  parser.addOption(log_file_option);
  const QCommandLineOption auto_install_option(
      "auto-install", "Enable auto-install mode", "", "");
  parser.addOption(auto_install_option);
  parser.addHelpOption();
  parser.addVersionOption();

  if (!parser.parse(args)) {
    qCritical() << "Failed to parse argument" << args;
    // Also print help text if QCoreApplication is initialized.
    if (QCoreApplication::instance()) {
      qCritical().noquote() << parser.helpText();
    }
    return false;
  }

  if (parser.isSet("version")) {
    if (QCoreApplication::instance()) {
      parser.showVersion();
    }
  }

  // Print help text and exit normally.
  if (parser.isSet("help")) {
    if (QCoreApplication::instance()) {
      parser.showHelp(0);
      return true;
    } else {
      // NOTE(xushaohua): Return false to notify caller to exit.
      qCritical().noquote() << parser.helpText();
      return false;
    }
  }

  if (parser.isSet(auto_install_option)) {
    auto_install_ = true;
  }

  if (parser.isSet(conf_file_option)) {
    conf_file_ = parser.value(conf_file_option);
  }

  if (parser.isSet(log_file_option)) {
    log_file_ = parser.value(log_file_option);
  }

  return true;
}

bool InstallerArgsParser::isAutoInstallSet() const {
  return auto_install_;
}

QString InstallerArgsParser::getConfFile() const {
  return conf_file_;
}

QString InstallerArgsParser::getLogFile() const {
  return log_file_;
}

}  // namespace installer