/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#ifdef NDEBUG
  // Scan wireless spot only in release mode.
  if (use_regdomain) {
    if (HasRootPrivilege()){
    emit wifi_inspect_worker_->scan();
    } else {
      qWarning() << "LocalManager requires root privilege!";
    }
  }
#else
  Q_UNUSED(use_regdomain);
#endif
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