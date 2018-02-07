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

#include "service/backend/geoip_request_worker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "base/string_util.h"

namespace installer {

namespace {

// We use geo lookup api from ubuntu.
const char kRequestUrl[] = "http://geoip.ubuntu.com/lookup";

// Parse timezone from http reply
bool ReadTimezone(QNetworkReply* reply, QString& timezone) {
  timezone = RegexpLabel("<TimeZone>([^<]+)", QString(reply->readAll()));
  return (!timezone.isEmpty());
}

}  // namespace

GeoIpRequestWorker::GeoIpRequestWorker(QObject* parent)
    : QObject(parent) {
  this->setObjectName("geo_ip_request_worker");

  network_manager_ = new QNetworkAccessManager(this);
  connect(this, &GeoIpRequestWorker::request,
          this, &GeoIpRequestWorker::doRequest);
  connect(network_manager_, &QNetworkAccessManager::finished,
          this, &GeoIpRequestWorker::onReplyFinished);
}

void GeoIpRequestWorker::doRequest() {
  const QUrl url(kRequestUrl);
  QNetworkReply* reply = network_manager_->get(QNetworkRequest(url));
  Q_UNUSED(reply);
}

void GeoIpRequestWorker::onReplyFinished(QNetworkReply* reply) {
  if (reply->error() == QNetworkReply::NoError) {
    QString timezone;
    if (ReadTimezone(reply, timezone)) {
      emit this->geoIpUpdated(timezone);
    } else {
      qWarning() << "Read timezone failed!";
    }
  } else {
    qWarning() << "Request timezone failed!";
  }
  reply->deleteLater();
}

}  // namespace installer