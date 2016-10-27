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

// Convert regdomain to timezone.
// NOTE(xushaohua): Some countries may have multiple timezones,
// like Russia and US. So this function is not quite reliable.
bool RegdomainToTimezone(const QString& regdomain, QString& timezone) {
  const ZoneInfoList list = GetZoneInfoList();
  for (const ZoneInfo& zone_info : list) {
    if (zone_info.country_code == regdomain) {
      timezone = zone_info.timezone;
      return true;
    }
  }
  return false;
}

}  // namespace

TimezoneManager::TimezoneManager(QObject* parent)
    : QObject(parent),
      wifi_inspect_thread_(new QThread(this)),
      geoip_request_thread_(new QThread(this)),
      geoip_request_is_replied_(false) {
  this->setObjectName(QStringLiteral("locale_manager"));

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
  delete wifi_inspect_worker_;
  wifi_inspect_worker_ = nullptr;

  wifi_inspect_thread_->quit();
  wifi_inspect_thread_->wait(3);
  delete wifi_inspect_thread_;
  wifi_inspect_thread_ = nullptr;

  delete geoip_worker_;
  geoip_worker_ = nullptr;

  geoip_request_thread_->quit();
  geoip_request_thread_->wait(3);
  delete geoip_request_thread_;
  geoip_request_thread_ = nullptr;
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
  if (!geoip_request_is_replied_) {
    // Guess timezone based on regdomain.
    QString timezone;
    if (RegdomainToTimezone(regdomain, timezone)) {
      emit this->timezoneUpdated(timezone);
    } else {
      qWarning() << "Failed to convert regdomain to timezone:" << regdomain;
    }
  } else {
    qDebug() << "GeoIP request got reply, so ignore wifi regdomain!";
  }
}

void TimezoneManager::onGeoIpUpdated(const QString& timezone) {
  geoip_request_is_replied_ = true;
  emit this->timezoneUpdated(timezone);
}

}  // namespace installer