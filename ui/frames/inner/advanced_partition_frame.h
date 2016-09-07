// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
class QPushButton;
class QVBoxLayout;

namespace ui {

class PartitionDelegate;

class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(PartitionDelegate* partition_delegate,
                         QWidget* parent = nullptr);

 signals:
  void requestEditPartitionFrame(const QString& partition_path);
  // TODO(xushaohua): Pass through a Partition object.
  void requestNewPartitionFrame(const QString& partition_path);
  void requestSelectBootloaderFrame();

 private:
  void initConnections();
  void initUI();
  void drawDevices();

  PartitionDelegate* partition_delegate_ = nullptr;

  QVBoxLayout* partition_layout_ = nullptr;
  QPushButton* bootloader_selection_button_ = nullptr;
  QPushButton* enable_editing_button_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
