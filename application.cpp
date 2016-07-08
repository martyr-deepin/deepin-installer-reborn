// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "application.h"

#include "service/signal_manager.h"
#include "ui/main_window.h"

Application::~Application() {
  if (main_window) {
    delete main_window;
    main_window = nullptr;
  }

  if (signal_manager) {
    delete signal_manager;
    signal_manager = nullptr;
  }
}

void Application::initServiceObjects() {
  signal_manager = new service::SignalManager();
}

void Application::initMainWindow() {
  main_window = new ui::MainWindow();
}