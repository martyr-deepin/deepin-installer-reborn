// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/signal_manager.h"

namespace service {

SignalManager::SignalManager() : QObject() {
  this->setObjectName(QStringLiteral("signal_manager"));
}

}  // namespace service