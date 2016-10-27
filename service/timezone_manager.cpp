// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/timezone_manager.h"

#include <QDebug>
#include <QThread>

#include "service/backend/wifi_inspect_worker.h"
#include "service/backend/geoip_request_worker.h"
#include "sysinfo/users.h"

namespace installer {

namespace {

// Number of country code.
const int kCountryCodeLen = 247;

// Map country code to timezone.
// See https://en.wikipedia.org/wiki/List_of_country_calling_codes
const char kCountryCodePair[][32] = {
    "AD", "Europe/Andorra",
    "AE", "",
    "AF", "",
    "AG", "",
    "AI", "",
    "AL", "",
    "AM", "",
    "AO", "",
    "AQ", "",
    "AR", "",
    "AS", "",
    "AT", "",
    "AU", "",
    "AW", "",
    "AX", "",
    "AZ", "",
    "BA", "",
    "BB", "",
    "BD", "",
    "BE", "",
    "BF", "",
    "BG", "",
    "BH", "",
    "BI", "",
    "BJ", "",
    "BL", "",
    "BM", "",
    "BN", "",
    "BO", "",
    "BQ", "",
    "BR", "",
    "BS", "",
    "BT", "",
    "BW", "",
    "BY", "",
    "BZ", "",
    "CA", "",
    "CC", "",
    "CD", "",
    "CF", "",
    "CG", "",
    "CH", "",
    "CI", "",
    "CK", "",
    "CL", "",
    "CM", "",
    "CN", "Asia/Shanghai",
    "CO", "",
    "CR", "",
    "CU", "",
    "CV", "",
    "CW", "",
    "CX", "",
    "CY", "",
    "CZ", "",
    "DE", "",
    "DJ", "",
    "DK", "",
    "DM", "",
    "DO", "",
    "DZ", "",
    "EC", "",
    "EE", "",
    "EG", "",
    "EH", "",
    "ER", "",
    "ES", "",
    "ET", "",
    "FI", "",
    "FJ", "",
    "FK", "",
    "FM", "",
    "FO", "",
    "FR", "",
    "GA", "",
    "GB", "",
    "GD", "",
    "GE", "",
    "GF", "",
    "GG", "",
    "GH", "",
    "GI", "",
    "GL", "",
    "GM", "",
    "GN", "",
    "GP", "",
    "GQ", "",
    "GR", "",
    "GS", "",
    "GT", "",
    "GU", "",
    "GW", "",
    "GY", "",
    "HK", "",
    "HN", "",
    "HR", "",
    "HT", "",
    "HU", "",
    "ID", "",
    "IE", "",
    "IL", "",
    "IM", "",
    "IN", "",
    "IO", "",
    "IQ", "",
    "IR", "",
    "IS", "",
    "IT", "",
    "JE", "",
    "JM", "",
    "JO", "",
    "JP", "",
    "KE", "",
    "KG", "",
    "KH", "",
    "KI", "",
    "KM", "",
    "KN", "",
    "KP", "",
    "KR", "",
    "KW", "",
    "KY", "",
    "KZ", "",
    "LA", "",
    "LB", "",
    "LC", "",
    "LI", "",
    "LK", "",
    "LR", "",
    "LS", "",
    "LT", "",
    "LU", "",
    "LV", "",
    "LY", "",
    "MA", "",
    "MC", "",
    "MD", "",
    "ME", "",
    "MF", "",
    "MG", "",
    "MH", "",
    "MK", "",
    "ML", "",
    "MM", "",
    "MN", "",
    "MO", "",
    "MP", "",
    "MQ", "",
    "MR", "",
    "MS", "",
    "MT", "",
    "MU", "",
    "MV", "",
    "MW", "",
    "MX", "",
    "MY", "",
    "MZ", "",
    "NA", "",
    "NC", "",
    "NE", "",
    "NF", "",
    "NG", "",
    "NI", "",
    "NL", "",
    "NO", "",
    "NP", "",
    "NR", "",
    "NU", "",
    "NZ", "",
    "OM", "",
    "PA", "",
    "PE", "",
    "PF", "",
    "PG", "",
    "PH", "",
    "PK", "",
    "PL", "",
    "PM", "",
    "PN", "",
    "PR", "",
    "PS", "",
    "PT", "",
    "PW", "",
    "PY", "",
    "QA", "",
    "RE", "",
    "RO", "",
    "RS", "",
    "RU", "",
    "RW", "",
    "SA", "",
    "SB", "",
    "SC", "",
    "SD", "",
    "SE", "",
    "SG", "",
    "SH", "",
    "SI", "",
    "SJ", "",
    "SK", "",
    "SL", "",
    "SM", "",
    "SN", "",
    "SO", "",
    "SR", "",
    "SS", "",
    "ST", "",
    "SV", "",
    "SX", "",
    "SY", "",
    "SZ", "",
    "TC", "",
    "TD", "",
    "TF", "",
    "TG", "",
    "TH", "",
    "TJ", "",
    "TK", "",
    "TL", "",
    "TM", "",
    "TN", "",
    "TO", "",
    "TR", "",
    "TT", "",
    "TV", "",
    "TW", "",
    "TZ", "",
    "UA", "",
    "UG", "",
    "UM", "",
    "US", "",
    "UY", "",
    "UZ", "",
    "VA", "",
    "VC", "",
    "VE", "",
    "VG", "",
    "VI", "",
    "VN", "",
    "VU", "",
    "WF", "",
    "WS", "",
    "YE", "",
    "YT", "",
    "ZA", "",
    "ZM", "",
    "ZW", "",
 };

// Convert regdomain to timezone.
// NOTE(xushaohua): Some countries may have multiple timezones,
// like Russia and US. So this function is not quite reliable.
bool RegdomainToTimezone(const QString& regdomain, QString& timezone) {
  for (int i = 0; i < kCountryCodeLen; i += 2) {
    if (regdomain == kCountryCodePair[i]) {
      timezone = kCountryCodePair[i+1];
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