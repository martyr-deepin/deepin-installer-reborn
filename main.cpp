// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QDebug>

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "sysinfo/users.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  installer::InitLogService();

  QApplication app(argc, argv);
  // TODO(xushaohua): Set i18n
  app.setApplicationDisplayName(installer::kAppDisplayName);
  app.setApplicationName(installer::kAppName);
  app.setOrganizationDomain(installer::kDomainName);

  // Delete last installer config file as soon as possible.
  if (!installer::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";
  }
  installer::DeleteConfigFile();
  // NOTE(xushaohua): Scans usb devices here, might be very slow.
  // TODO(xushaohua): Add a startup option to disable scanning operation.
  installer::SaveOemConfig();

  installer::MainWindow main_window;
  main_window.fullscreen();

  const int result = app.exec();
  return result;
}