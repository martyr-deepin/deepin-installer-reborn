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

#ifndef INSTALLER_WIFI_INSPECT_WORKER_H
#define INSTALLER_WIFI_INSPECT_WORKER_H

#include <QObject>

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
