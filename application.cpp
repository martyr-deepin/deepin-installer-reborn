// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "application.h"

#include "base/consts.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "service/signal_manager.h"
#include "ui/main_window.h"

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {
  this->setObjectName(QStringLiteral("app"));
  this->initAppInfo();
}

Application::~Application() {
  if (main_window) {
    delete main_window;
    main_window = nullptr;
  }

  if (settings_manager) {
    delete settings_manager;
    settings_manager = nullptr;
  }

  if (signal_manager) {
    delete signal_manager;
    signal_manager = nullptr;
  }
}

void Application::initAppInfo() {
  // TODO(xushaohua): Set i18n

  this->setApplicationDisplayName(base::kAppDisplayName);
  this->setApplicationName(base::kAppName);
  this->setOrganizationDomain(QStringLiteral("deepin.org"));
}

void Application::initMainWindow() {
  main_window = new ui::MainWindow();
}

void Application::initServiceObjects() {
  service::InitLogService();

  signal_manager = new service::SignalManager();
  settings_manager = new service::SettingsManager();
}
