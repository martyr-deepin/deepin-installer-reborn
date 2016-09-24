// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
class QButtonGroup;
class QVBoxLayout;

#include "partman/partition.h"

namespace ui {

class FlatButton;
class PartitionDelegate;

class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(PartitionDelegate* partition_delegate,
                         QWidget* parent = nullptr);

 signals:
  void requestEditPartitionFrame(const partman::Partition& partition);
  void requestNewPartitionFrame(const partman::Partition& partition);
  void requestSelectBootloaderFrame();

 private:
  void initConnections();
  void initUI();
  void drawDevices();

  PartitionDelegate* delegate_ = nullptr;

  QButtonGroup* partition_button_group_ = nullptr;
  QVBoxLayout* partition_layout_ = nullptr;
  FlatButton* bootloader_selection_button_ = nullptr;
  FlatButton* enable_editing_button_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
