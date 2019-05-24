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

// Run this program to setup username, password, keyboard and timezone after
// reboot system. Only if `system_info_setup_after_reboot` flag is enabled.

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTranslator>

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "sysinfo/users.h"
#include "ui/delegates/language_delegate.h"
#include "ui/first_boot_setup_window.h"

#include <cstdlib>
#include "base/auto_screen_scale.h"

int main(int argc, char *argv[])
{
  Utils::AutoScreenScale();
  QApplication app(argc, argv);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);
  app.setAttribute(Qt::AA_EnableHighDpiScaling);
  app.setApplicationDisplayName("Deepin Installer First Boot");
  app.setApplicationName("deepin-installer-first-boot");
  app.setApplicationVersion(installer::kAppVersion);
  app.setOrganizationDomain(installer::kOrganizationDomain);
  app.setWindowIcon(QIcon(":/images/deepin_installer.svg"));

  const char kLogFileName[] = "deepin-installer-first-boot.log";
  QString log_file;
  if (!installer::HasRootPrivilege())
  {
    qCritical() << "Root privilege is required!";
    log_file = QString("/tmp/%1").arg(kLogFileName);
  }
  else
  {
    log_file = QString("/var/log/%1").arg(kLogFileName);
  }
  // Initialize log service.
  installer::RedirectLog(log_file);

  // Set language.
  QTranslator translator;
  const QString locale(installer::ReadLocale());
  translator.load(installer::GetLocalePath(locale));
  app.installTranslator(&translator);

  QFont font(app.font());
  font.setFamily(installer::GetUIDefaultFont());
  app.setFont(font);

  installer::FirstBootSetupWindow window;
  window.fullscreen();

  return app.exec();
}
