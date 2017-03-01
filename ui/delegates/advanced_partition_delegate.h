// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H

#include <QObject>

namespace installer {

// Partition delegate used in AdvancedPartitionFrame and other sub frame pages.
class AdvancedPartitionDelegate : public QObject {
  Q_OBJECT
 public:
  explicit AdvancedPartitionDelegate(QObject* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
