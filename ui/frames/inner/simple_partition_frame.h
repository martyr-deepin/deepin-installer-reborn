// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H

#include <QFrame>
class QAbstractButton;
class QButtonGroup;
class QGridLayout;
class QLabel;
class QShowEvent;

namespace installer {

class PartitionDelegate;

// List device partitions in simple mode.
class SimplePartitionFrame : public QFrame {
  Q_OBJECT

 public:
  SimplePartitionFrame(PartitionDelegate* delegate, QWidget* parent = nullptr);

  // Validate whether a root partition is selected and large enough.
  bool validate();

 protected:
  void changeEvent(QEvent* event) override;

  // Show install_tip if needed.
  void showEvent(QShowEvent* event) override;

 private:
  void initConnections();
  void initUI();

  void repaintDevices();

  // Show install_tip at bottom of |button|.
  void showInstallTip(QAbstractButton* button);

  QButtonGroup* button_group_ = nullptr;
  QFrame* install_tip_ = nullptr;
  QLabel* tip_label_ = nullptr;
  QGridLayout* grid_layout_ = nullptr;
  PartitionDelegate* delegate_ = nullptr;

  // Displays error message in validate().
  // It is cleared when another SimplePartitionButton is selected.
  QLabel* msg_label_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onPartitionButtonToggled(QAbstractButton* button, bool checked);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
