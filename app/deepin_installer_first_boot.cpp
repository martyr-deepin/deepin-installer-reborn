// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Run this program to setup username, password, keyboard and timezone after
// reboot system. Only if `system_info_setup_after_reboot` flag is enabled.

#include <QApplication>
#include <QDebug>
#include <QIcon>

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "sysinfo/users.h"
#include "ui/first_boot_setup_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName(installer::kAppDisplayName);
  app.setApplicationName(installer::kAppName);
  app.setOrganizationDomain(installer::kDomainName);
  app.setWindowIcon(QIcon(":/images/deepin_installer_reborn.svg"));

  // Delete last installer config file as soon as possible.
  if (!installer::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";
  }

  installer::FirstBootSetupWindow window;
  window.fullscreen();

  return app.exec();
}