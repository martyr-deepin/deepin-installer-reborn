// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/wifi_inspect_worker.h"

namespace installer {

WiFiInspectWorker::WiFiInspectWorker(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("wifi_inspect_worker"));
}

}  // namespace installer