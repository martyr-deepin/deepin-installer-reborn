// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
class QButtonGroup;
class QVBoxLayout;

#include "partman/partition.h"

namespace installer {

class FlatButton;
class PartitionDelegate;

class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(PartitionDelegate* delegate_,
                         QWidget* parent = nullptr);

  void repaintDevices();

 signals:
  void requestEditPartitionFrame(const Partition& partition);
  void requestNewPartitionFrame(const Partition& partition);
  void requestSelectBootloaderFrame();

 private:
  void initConnections();
  void initUI();

  PartitionDelegate* delegate_ = nullptr;

  QButtonGroup* partition_button_group_ = nullptr;
  QVBoxLayout* partition_layout_ = nullptr;
  FlatButton* bootloader_selection_button_ = nullptr;
  FlatButton* enable_editing_button_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
