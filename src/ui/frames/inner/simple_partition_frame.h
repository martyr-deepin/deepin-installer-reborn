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

#ifndef INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H

#include <QFrame>
class QAbstractButton;
class QButtonGroup;
class QGridLayout;
class QLabel;
class QShowEvent;

#include "partman/partition.h"
#include "partman/device.h"

namespace installer {

class SimplePartitionDelegate;

// List device partitions in simple mode.
class SimplePartitionFrame : public QFrame {
  Q_OBJECT

 public:
  SimplePartitionFrame(SimplePartitionDelegate* delegate,
                       QWidget* parent = nullptr);

  // Validate whether a root partition is selected and large enough.
  bool validate();

  Device::Ptr selectedDevice() const;

 signals:
  // Emitted when partition table not appropriate at |device_path|.
  void requestNewTable(const QString& device_path);

 protected:
  void changeEvent(QEvent* event) override;

  // Show install_tip if needed.
  void showEvent(QShowEvent* event) override;

 private:
  // Create and append operations into delegate:
  //  * A root partition;
  //  * A linux-swap file if no linux-swap partition is found;
  //  * An EFI partition if EFI mode is on and no EFI partition is found;
  // Call this method only if SimplePartitionFrame is represented in
  // PartitionFrame. And call validate() first.
  void appendOperations();

  void initConnections();
  void initUI();

  // Repaint partition list.
  void repaintDevices();

  // Show install_tip at bottom of |button|.
  void showInstallTip(QAbstractButton* button);

  QButtonGroup* button_group_ = nullptr;
  QFrame* install_tip_ = nullptr;
  QLabel* tip_label_ = nullptr;
  QFrame* grid_wrapper_ = nullptr;
  QGridLayout* grid_layout_ = nullptr;
  SimplePartitionDelegate* delegate_ = nullptr;

  // Displays error message in validate().
  // It is cleared when another SimplePartitionButton is selected.
  QLabel* msg_label_ = nullptr;

 private slots:
  void onDeviceRefreshed();

  void onPartitionButtonClicked();
  // Di-select the unchecked button.
  void onPartitionButtonToggled(QAbstractButton* button, bool checked);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SIMPLE_PARTITION_FRAME_H
