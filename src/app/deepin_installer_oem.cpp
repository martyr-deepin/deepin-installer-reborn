/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// OEM program to modify settings file.

#include <QApplication>

#include "base/consts.h"
#include "oem/oem_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);
  app.setAttribute(Qt::AA_EnableHighDpiScaling);
  app.setApplicationDisplayName("Deepin Installer Oem Tool");
  app.setApplicationName("deepin-installer-oem");
  app.setApplicationVersion(installer::kAppVersion);
  app.setOrganizationDomain(installer::kOrganizationDomain);

  installer::OemWindow oem_window;
  if (!oem_window.init()) {
    return 1;
  }
  oem_window.resize(860, 640);
  oem_window.show();

  return app.exec();
}