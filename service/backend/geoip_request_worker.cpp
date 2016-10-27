// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  this->setObjectName(QStringLiteral("geo_ip_request_worker"));

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