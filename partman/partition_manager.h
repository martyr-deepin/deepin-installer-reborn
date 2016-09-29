// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_MANAGER_H

#include <QList>
#include <QObject>

#include "partman/device.h"
#include "partman/operation.h"

namespace partman {

class PartitionManager : public QObject {
  Q_OBJECT

 public:
  explicit PartitionManager(QObject* parent = nullptr);
  ~PartitionManager();

 signals:
  void refreshDevices();
  void devicesRefreshed(const DeviceList& devices);

  // Run auto part script at |script_path|.
  void autoPart(const QString& script_path);
  // Emitted after oem/auto-part.sh script is executed and exited.
  // |ok| is true if that script exited 0.
  void autoPartDone(bool ok);

  void manualPart(const OperationList& operations);
  // |mount_points| is a list of partition_path -> mount_point pair.
  void manualPartDone(bool ok,
                      const QList<QPair<QString, QString>>& mount_point_pair);

 private:
  void initConnections();

 private slots:
  void doRefreshDevices();
  void doAutoPart(const QString& script_path);
  void doManualPart(const OperationList& operations);
};

// Scan all disk devices on this machine.
// Do not call this function directly, use PartitionManager instead.
DeviceList ScanDevices();

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_MANAGER_H
