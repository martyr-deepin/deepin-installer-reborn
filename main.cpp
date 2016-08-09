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
//  service::RedirectLogFile();

  QApplication app(argc, argv);
  // TODO(xushaohua): Set i18n
  app.setApplicationDisplayName(base::kAppDisplayName);
  app.setApplicationName(base::kAppName);
  app.setOrganizationDomain(QStringLiteral("deepin.org"));

  // Delete last installer config file as soon as possible.
  service::DeleteConfigFile();
  if (!sysinfo::HasRootPrivilege()) {
    qCritical() << "Root privilege is required!";
  }

  ui::MainWindow main_window;
  main_window.showFullScreen();

  const int result = app.exec();
//  service::ShutdownLogService();
  return result;
}