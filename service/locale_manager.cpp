// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/locale_manager.h"

#include <QDebug>
#include <QThread>

#include "service/backend/wifi_inspect_worker.h"
#include "sysinfo/users.h"

namespace installer {

LocaleManager::LocaleManager(QObject* parent)
    : QObject(parent),
      wifi_inspect_thread_(new QThread(this)) {
  this->setObjectName(QStringLiteral("locale_manager"));

  wifi_inspect_thread_->start();
  wifi_inspect_worker_ = new WiFiInspectWorker();
  connect(wifi_inspect_worker_, &WiFiInspectWorker::regdomainUpdated,
          this, &LocaleManager::onRegdomainUpdated);
  wifi_inspect_worker_->moveToThread(wifi_inspect_thread_);
}

LocaleManager::~LocaleManager() {
  delete wifi_inspect_worker_;
  wifi_inspect_worker_ = nullptr;

  wifi_inspect_thread_->quit();
  wifi_inspect_thread_->wait(3);
  delete wifi_inspect_thread_;
  wifi_inspect_thread_ = nullptr;
}

void LocaleManager::update() {
  if (HasRootPrivilege()){
    emit wifi_inspect_worker_->scan();
  } else {
    qWarning() << "LocalManager requires root privilege!";
  }
}

void LocaleManager::onRegdomainUpdated(const QString& regdomain) {
  qDebug() << "on regdomain updated:"<< regdomain;
  emit this->timezoneUpdated(regdomain);
}

}  // namespace installer