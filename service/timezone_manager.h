// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  WiFiInspectWorker* wifi_inspect_worker_ = nullptr;
  GeoIpRequestWorker* geoip_worker_ = nullptr;
  QThread* wifi_inspect_thread_ = nullptr;
  QThread* geoip_request_thread_ = nullptr;

  bool geoip_request_is_replied_;

 private slots:
  void onGeoIpUpdated(const QString& timezone);

  // Convert regdomain to locale.
  void onRegdomainUpdated(const QString& regdomain);
};

} // namespace installer

#endif  // INSTALLER_LOCALE_MANAGER_H
