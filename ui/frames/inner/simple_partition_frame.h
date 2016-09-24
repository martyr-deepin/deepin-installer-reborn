// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H

#include <QScrollArea>
class QAbstractButton;
class QButtonGroup;

namespace ui {

class PartitionDelegate;

class SimplePartitionFrame : public QScrollArea {
  Q_OBJECT

 public:
  SimplePartitionFrame(PartitionDelegate* partition_delegate,
                       QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();
  void drawDevices();

  QButtonGroup* partition_button_group_ = nullptr;
  QFrame* install_tip_ = nullptr;
  PartitionDelegate* delegate_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onPartitionButtonToggled(QAbstractButton* button, bool checked);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
