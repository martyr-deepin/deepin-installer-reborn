// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "base/consts.h"
#include "service/log_manager.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // TODO(xushaohua): Set i18n
  app.setApplicationDisplayName(base::kAppDisplayName);
  app.setApplicationName(base::kAppName);
  app.setOrganizationDomain(QStringLiteral("deepin.org"));

//  service::InitLogService();

  ui::MainWindow main_window;
  main_window.showFullScreen();

  return app.exec();
}