// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>

namespace ui {

class PartitionDelegate;

class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(PartitionDelegate* partition_delegate,
                         QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  PartitionDelegate* partition_delegate_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
