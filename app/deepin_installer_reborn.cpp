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
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  // Reset locale to en_US.
  // NOTE(xushaohua): "LANG" might not set in some live environment.
  qputenv("LC_ALL", installer::kDefaultLocale);
  qputenv("LANG", installer::kDefaultLocale);

  QApplication app(argc, argv);
  app.setApplicationDisplayName(installer::kAppDisplayName);
  app.setApplicationName(installer::kAppName);
  app.setOrganizationDomain(installer::kDomainName);
  app.setWindowIcon(QIcon(":/images/deepin_installer_reborn.svg"));

  // Delete last installer config file as soon as possible.
  if (!installer::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";

#ifdef NDEBUG
    // Returns immediately if process permission is error.
    return 1;
#endif
  }

  // Initialize log service.
  installer::InitLogService();

  // Delete old settings file and generate a new one.
  installer::DeleteConfigFile();
  installer::AddConfigFile();

  installer::MainWindow main_window;

  // Notify background thread to scan device info.
  main_window.scanDevicesAndTimezone();

  main_window.fullscreen();

  return app.exec();
}
