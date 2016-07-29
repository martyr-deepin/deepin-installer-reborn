// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H

#include <QObject>
#include <QList>

namespace service {

struct Partition {
  // File system
  QString fs;  // file system type.
  qint64 freespace;
  qint64 length;
  QString label;
  QString uuid;
  QStringList mounts;

  // Partition
  QString path;
  qint64 first_sector;
  qint64 last_sector;
  qint64 total_sectors;
  QString name;
  QStringList flags;
  QString os;
};

struct Device {
  QString model;
  QString path;
  QString table; // partition table.
  qint64 freespace;
  qint64 length;  // total sectors / disk capacity.
  int heads;
  int sectors;
  int cylinders;
  qint64 sector_size;
  QList<Partition> partitions;
};

class PartitionManager : public QObject {
  Q_OBJECT

 public:
  explicit PartitionManager(QObject* parent = nullptr);

 signals:
  void refreshDevices();
  void devicesRefreshed();

 private:
  void initConnections();

 private slots:
  void doRefreshDevices();
};

// Check if EFI feature is enabled in this machine.
bool EfiIsEnabled();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_H
