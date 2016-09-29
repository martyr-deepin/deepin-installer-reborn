// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "base/consts.h"
#include "misc/oem/oem_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName(installer::kAppDisplayName);
  app.setApplicationName(installer::kAppName);
  app.setOrganizationDomain(installer::kDomainName);

  installer::OemWindow oem_window;
  oem_window.show();

  return app.exec();
}