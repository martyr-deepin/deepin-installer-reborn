// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H

#include <QObject>

namespace installer {

// Partition delegate used in SimplePartitionFrame.
class SimplePartitionDelegate : public QObject {
  Q_OBJECT
 public:
  explicit SimplePartitionDelegate(QObject* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
