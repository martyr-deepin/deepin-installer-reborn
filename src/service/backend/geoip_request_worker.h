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
