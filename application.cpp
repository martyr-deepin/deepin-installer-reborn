// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "application.h"

#include "service/signal_manager.h"

Application::~Application() {
  if (signal_manager) {
    delete signal_manager;
    signal_manager = nullptr;
  }
}

void Application::initServiceObjects() {
  signal_manager = new service::SignalManager();
}

