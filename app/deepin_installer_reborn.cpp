// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Main program of installer.

#include <QApplication>
#include <QDebug>
#include <QIcon>

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "sysinfo/users.h"
#include "ui/delegates/installer_args_parser.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  // Reset LC_ALL to en_US.UTF-8.
  // NOTE(xushaohua): "LANG" might not set in some live environment.
  qputenv("LC_ALL", installer::kDefaultLang);
  qputenv("LANG", installer::kDefaultLang);

  QApplication app(argc, argv);
  app.setApplicationDisplayName("Deepin Installer Reborn");
  app.setApplicationName("deepin-installer-reborn");
  app.setApplicationVersion(installer::kAppVersion);
  app.setOrganizationDomain(installer::kOrganizationDomain);
  app.setWindowIcon(QIcon(":/images/deepin_installer_reborn.svg"));

  // Parse argument list.
  installer::InstallerArgsParser args_parser;
  // Returns false if failed to parse arguments.
  if (!args_parser.parse(app.arguments())) {
    return 1;
  }

  // Initialize log service.
  const char kLogFileName[] = "deepin-installer-reborn.log";
  QString log_file;
  if (!installer::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";
    log_file = QString("/tmp/%1").arg(kLogFileName);
  } else {
    log_file = QString("/var/log/%1").arg(kLogFileName);
  }
  installer::RedirectLog(log_file);

  // Delete old settings file and generate a new one.
  installer::DeleteConfigFile();
  installer::AddConfigFile();

  qDebug() << "Version:" << installer::kAppVersion;

  const QString conf_file(args_parser.getConfFile());
  // Append customized configurations.
  if (!conf_file.isEmpty()) {
    if (!installer::AppendConfigFile(conf_file)) {
      qCritical() << "Failed to append conf file:" << conf_file;
    }
  }

  installer::MainWindow main_window;
  main_window.setEnableAutoInstall(args_parser.isAutoInstallSet());
  main_window.setLogFile(args_parser.getLogFile());
  // Notify background thread to scan device info.
  main_window.scanDevicesAndTimezone();
  main_window.fullscreen();

  return app.exec();
}
