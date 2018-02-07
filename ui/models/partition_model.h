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

#ifndef INSTALLER_UI_MODELS_PARTITION_MODEL_H
#define INSTALLER_UI_MODELS_PARTITION_MODEL_H

#include <QList>
#include <QObject>
class QThread;

#include "partman/device.h"
#include "partman/operation.h"

namespace installer {

class PartitionManager;

// Communication with partman/PartitionManager.
// Both SimplePartitionDelegate and AdvancedPartitionDelegate read real device
// list and send operation list to this class.
class PartitionModel : public QObject {
  Q_OBJECT

 public:
  explicit PartitionModel(QObject* parent = nullptr);
  ~PartitionModel();

 signals:
  // Emitted when auto partitioning job is done.
  void autoPartDone(bool ok);

  // Emitted after scanning local disk devices.
  void deviceRefreshed(const DeviceList& devices);

  // Emitted when manual partitioning job is done.
  void manualPartDone(bool ok, const DeviceList& devices);

 public slots:
  // Notify PartitionManager to do auto-part
  void autoPart();

  // Create partition table at |device_path| to match current booting method.
  // After that, emit deviceRefreshed() signal.
  void createPartitionTable(const QString& device_path);

  // Do manual partitioning based on these |operations|.
  void manualPart(const OperationList& operations);

  // Notifies partition manager to scan devices.
  void scanDevices();

 private:
  void initConnections();

  PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PARTITION_MODEL_H
