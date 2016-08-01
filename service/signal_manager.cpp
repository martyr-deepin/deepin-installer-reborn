// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/signal_manager.h"

namespace service {

SignalManager* g_signal_manager = nullptr;

SignalManager::SignalManager() : QObject() {
  this->setObjectName(QStringLiteral("signal_manager"));
}

// static
SignalManager* SignalManager::instance() {
  if (g_signal_manager == nullptr) {
    g_signal_manager = new SignalManager();
  }
  return g_signal_manager;
}

}  // namespace service