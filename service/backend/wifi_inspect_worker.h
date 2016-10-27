// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_WIFI_INSPECT_WORKER_H
#define INSTALLER_WIFI_INSPECT_WORKER_H

#include <QObject>
#include <QStringList>

namespace installer {

// Scan wifi access points and parse country code (regdomain) property from
// beacon packets.
class WiFiInspectWorker : public QObject {
  Q_OBJECT

 public:
  explicit WiFiInspectWorker(QObject* parent = nullptr);

 signals:
  // Start scanning process.
  void scan();

  // Emitted when tshark finished.
  // |regdomain| is a two-character string defined in ISO 3161-1,
  // like "CN", "US"
  void regdomainUpdated(const QString& regdomain);

 private slots:
  void doScanning();
};

}  // namespace installer

#endif  // INSTALLER_WIFI_INSPECT_WORKER_H
