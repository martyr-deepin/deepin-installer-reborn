// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/locale_request_worker.h"

namespace installer {

LocaleRequestWorker::LocaleRequestWorker(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("locale_request_worker"));
}

}  // namespace installer