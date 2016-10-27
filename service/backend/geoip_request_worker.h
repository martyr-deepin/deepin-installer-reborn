// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_LOCALE_REQUEST_WORKER_H
#define INSTALLER_LOCALE_REQUEST_WORKER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

namespace installer {

// Send http request to web server to retrieve locale information about
// current IP.
class GeoIpRequestWorker : public QObject {
  Q_OBJECT

 public:
  explicit GeoIpRequestWorker(QObject* parent = nullptr);

 signals:
  // Notify GeoIpRequestWorker to send http request.
  void request();

  // Emitted when received response from server.
  void geoIpUpdated(const QString& locale);

 private:
  QNetworkAccessManager* network_manager_ = nullptr;

 private slots:
  void doRequest();
  void onReplyFinished(QNetworkReply* reply);
};

}  // namespace installer

#endif  // INSTALLER_LOCALE_REQUEST_WORKER_H
