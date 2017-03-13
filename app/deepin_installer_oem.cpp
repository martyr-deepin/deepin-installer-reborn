// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// OEM program to modify settings file.

#include <QApplication>

#include "base/consts.h"
#include "oem/oem_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setApplicationDisplayName("Deepin Installer Oem Tool");
  app.setApplicationName("deepin-installer-oem");
  app.setApplicationVersion(installer::kAppVersion);
  app.setOrganizationDomain(installer::kOrganizationDomain);

  installer::OemWindow oem_window;
  oem_window.resize(860, 640);
  oem_window.show();

  return app.exec();
}