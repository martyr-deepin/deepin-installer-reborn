// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/timezone_manager.h"

#include <QDebug>
#include <QThread>

#include "base/thread_util.h"
#include "service/backend/wifi_inspect_worker.h"
#include "service/backend/geoip_request_worker.h"
#include "sysinfo/timezone.h"
#include "sysinfo/users.h"

namespace installer {

namespace {

// Convert country code to timezone.
// NOTE(xushaohua): Some countries may have multiple timezones,
// like Russia and US. So this function is not quite reliable.
QString CountryCodeToTimezone(const QString& regdomain) {
  const ZoneInfoList list = GetZoneInfoList();
  const int index = GetZoneInfoByCountry(list, regdomain);
  if (index > -1) {
    return list.at(index).timezone;
  } else {
    return QString();
  }
}

}  // namespace

TimezoneManager::TimezoneManager(QObject* parent)
    : QObject(parent),
      geoip_worker_(new GeoIpRequestWorker()),
      wifi_inspect_worker_(new WiFiInspectWorker()),
      geoip_request_thread_(new QThread(this)),
      wifi_inspect_thread_(new QThread(this)) {
  this->setObjectName("locale_manager");

  geoip_worker_->moveToThread(geoip_request_thread_);
  wifi_inspect_worker_->moveToThread(wifi_inspect_thread_);

  this->initConnections();

  geoip_request_thread_->start();
  wifi_inspect_thread_->start();
}

TimezoneManager::~TimezoneManager() {
  QuitThread(geoip_request_thread_);
  QuitThread(wifi_inspect_thread_);
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

void TimezoneManager::initConnections() {
  connect(geoip_worker_, &GeoIpRequestWorker::geoIpUpdated,
          this, &TimezoneManager::onGeoIpUpdated);
  connect(wifi_inspect_worker_, &WiFiInspectWorker::regdomainUpdated,
          this, &TimezoneManager::onRegdomainUpdated);

  connect(geoip_request_thread_, &QThread::finished,
          geoip_worker_, &GeoIpRequestWorker::deleteLater);
  connect(wifi_inspect_thread_, &QThread::finished,
          wifi_inspect_worker_, &WiFiInspectWorker::deleteLater);

}

void TimezoneManager::onGeoIpUpdated(const QString& timezone) {
  qDebug() << "onGeoIpUpdated():" << timezone;
  emit this->timezoneUpdated(timezone);
}

void TimezoneManager::onRegdomainUpdated(const QString& regdomain) {
  qDebug() << "onRegdomainUpdated():" << regdomain;
  // Guess timezone based on regdomain.
  const QString timezone = CountryCodeToTimezone(regdomain);
  if (!timezone.isEmpty()) {
    emit this->timezoneUpdated(timezone);
  } else {
    qWarning() << "Failed to convert regdomain to timezone:" << regdomain;
  }
}

}  // namespace installer