// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/locale_manager.h"

namespace installer {

LocaleManager::LocaleManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("locale_manager"));
}

void LocaleManager::update() {

}

}  // namespace installer