// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/timezone_manager.h"

#include <QDebug>
#include <QThread>

#include "service/backend/wifi_inspect_worker.h"
#include "service/backend/geoip_request_worker.h"
#include "sysinfo/timezone.h"
#include "sysinfo/users.h"

namespace installer {

namespace {

// Convert country code to timezone.
// NOTE(xushaohua): Some countries may have multiple timezones,
// like Russia and US. So this function is not quite reliable.
bool CountryCodeToTimezone(const QString& regdomain, QString& timezone) {
  const ZoneInfoList list = GetZoneInfoList();
  const int index = GetZoneInfoByCountry(list, regdomain);
  if (index > -1) {
    timezone = list.at(index).timezone;
    return true;
  } else {
    return false;
  }
}

}  // namespace

TimezoneManager::TimezoneManager(QObject* parent)
    : QObject(parent),
      wifi_inspect_thread_(new QThread(this)),
      geoip_request_thread_(new QThread(this)) {
  this->setObjectName("locale_manager");

  wifi_inspect_thread_->start();
  wifi_inspect_worker_ = new WiFiInspectWorker();
  connect(wifi_inspect_worker_, &WiFiInspectWorker::regdomainUpdated,
          this, &TimezoneManager::onRegdomainUpdated);
  wifi_inspect_worker_->moveToThread(wifi_inspect_thread_);

  geoip_request_thread_->start();
  geoip_worker_ = new GeoIpRequestWorker();
  connect(geoip_worker_, &GeoIpRequestWorker::geoIpUpdated,
          this, &TimezoneManager::onGeoIpUpdated);
  geoip_worker_->moveToThread(geoip_request_thread_);
}

TimezoneManager::~TimezoneManager() {
  if (wifi_inspect_thread_) {
    wifi_inspect_thread_->quit();
    wifi_inspect_thread_->wait(3);
    delete wifi_inspect_thread_;
    wifi_inspect_thread_ = nullptr;
  }

  if (geoip_request_thread_) {
    geoip_request_thread_->quit();
    geoip_request_thread_->wait(3);
    delete geoip_request_thread_;
    geoip_request_thread_ = nullptr;
  }
}

void TimezoneManager::update(bool use_geoip, bool use_regdomain) {
  if (use_geoip) {
    geoip_worker_->request();
  }
  if (use_regdomain) {
    if (HasRootPrivilege()){
    emit wifi_inspect_worker_->scan();
    } else {
      qWarning() << "LocalManager requires root privilege!";
    }
  }
}

void TimezoneManager::onRegdomainUpdated(const QString& regdomain) {
  qDebug() << "onRegdomainUpdated():" << regdomain;
  // Guess timezone based on regdomain.
  QString timezone;
  if (CountryCodeToTimezone(regdomain, timezone)) {
    emit this->timezoneUpdated(timezone);
  } else {
    qWarning() << "Failed to convert regdomain to timezone:" << regdomain;
  }
}

void TimezoneManager::onGeoIpUpdated(const QString& timezone) {
  qDebug() << "onGeoIpUpdated():" << timezone;
  emit this->timezoneUpdated(timezone);
}

}  // namespace installer