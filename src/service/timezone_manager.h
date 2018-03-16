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

#ifndef INSTALLER_LOCALE_MANAGER_H
#define INSTALLER_LOCALE_MANAGER_H

#include <QObject>
class QThread;

namespace installer {

class GeoIpRequestWorker;
class WiFiInspectWorker;

// Retrieve timezone info based on geoip and regdomain.
// Use this object in UI thread because it manages requests in background
// threads.
class TimezoneManager : public QObject {
  Q_OBJECT

 public:
  explicit TimezoneManager(QObject* parent = nullptr);
  ~TimezoneManager();

  // Update timezone info.
  // It will emit |timezoneUpdated| signal on success.
  // Set |use_geoip| to true if using geoip lookup service.
  // Set |use_regdomain| to true if using regulatory domain code retrieved
  // from nearby access points.
  void update(bool use_geoip, bool use_regdomain);

 signals:
  void timezoneUpdated(const QString& timezone);

 private:
  void initConnections();

  GeoIpRequestWorker* geoip_worker_ = nullptr;
  WiFiInspectWorker* wifi_inspect_worker_ = nullptr;
  QThread* geoip_request_thread_ = nullptr;
  QThread* wifi_inspect_thread_ = nullptr;

 private slots:
  void onGeoIpUpdated(const QString& timezone);

  // Convert regdomain to locale.
  void onRegdomainUpdated(const QString& regdomain);
};

} // namespace installer

#endif  // INSTALLER_LOCALE_MANAGER_H
