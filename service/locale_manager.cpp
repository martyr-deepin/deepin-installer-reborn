// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/locale_manager.h"

#include <QDebug>

#include "service/backend/wifi_inspect_worker.h"
#include "sysinfo/users.h"

namespace installer {

LocaleManager::LocaleManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("locale_manager"));

  inspect_worker_ = new WiFiInspectWorker(this);

  connect(inspect_worker_, &WiFiInspectWorker::regdomainUpdated,
          this, &LocaleManager::onRegdomainUpdated);
}

LocaleManager::~LocaleManager() {

}

void LocaleManager::update() {
  if (HasRootPrivilege()){
    emit inspect_worker_->scan();
  } else {
    qWarning() << "LocalManager requires root privilege!";
  }
}

void LocaleManager::onRegdomainUpdated(const QString& regdomain) {
  qDebug() << "on regdomain updated:"<< regdomain;
  emit this->timezoneUpdated(regdomain);
}

}  // namespace installer