// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_PARTITION_MODEL_H
#define INSTALLER_UI_MODELS_PARTITION_MODEL_H

#include <QList>
#include <QObject>
class QThread;

#include "partman/device.h"
#include "partman/operation.h"

namespace installer {

// Communication with partman/PartitionManager.
// Both SimplePartitionDelegate and AdvancedPartitionDelegate read real device
// list and send operation list to this class.
class PartitionModel : public QObject {
  Q_OBJECT

 public:
  explicit PartitionModel(QObject* parent = nullptr);

 signals:
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PARTITION_MODEL_H
