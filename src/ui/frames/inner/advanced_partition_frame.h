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

#ifndef INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
#include <QVector>

class QButtonGroup;
class QLabel;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

#include "partman/partition.h"
#include "ui/delegates/advanced_partition_delegate.h"
#include "ui/widgets/advanced_partition_error_label.h"

namespace installer {

class AdvancedPartitionButton;

// Advanced partition mode
class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(AdvancedPartitionDelegate* delegate_,
                         QWidget* parent = nullptr);

  // Check whether partition operations are appropriate.
  // If partition operation is not satisfied, show error message list.
  // This method is called when next-button is clicked in PartitionFrame
  bool validate();

  QList<Device::Ptr> getAllUsedDevice() const;

 signals:
  void requestEditPartitionFrame(const Partition::Ptr partition);
  void requestNewPartitionFrame(const Partition::Ptr partition);

  // Emitted when partition table not appropriate at |device_path|.
  void requestNewTable(const QString& device_path);

  // Show PartitionNumberLimitationFrame.
  void requestPartitionNumberLimitationFrame();

  // Show SelectBootloaderFrame.
  void requestSelectBootloaderFrame();

 public slots:
  // Update bootloader path in button.
  // This slots is connected to SelectBootloaderFrame.
  void setBootloaderPath(const QString& bootloader_path);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Returns a partition button which can be used to fix error |state|.
  AdvancedPartitionButton* getAppropriateButtonForState(
      AdvancedValidateState state) const;

  void hideErrorMessage(AdvancedValidateState state);
  void hideErrorMessages();

  void repaintDevices();

  // Scroll to top of content area.
  void scrollContentToTop();

  void showErrorMessage(AdvancedValidateState state);
  // Show error message container.
  void showErrorMessages();

  // Update header text based on current states.
  void updateErrorMessageHeader();

  // Update error messages and validate states.
  void updateValidateStates();

  // Returns error message related to this |state|.
  QString validateStateToText(AdvancedValidateState state);

  AdvancedPartitionDelegate* delegate_ = nullptr;

  QPushButton* bootloader_tip_button_ = nullptr;
  QButtonGroup* partition_button_group_ = nullptr;
  QVBoxLayout* partition_layout_ = nullptr;
  QPushButton* bootloader_button_ = nullptr;
  QPushButton* editing_button_ = nullptr;

  QScrollArea* scroll_area_ = nullptr;
  QFrame* msg_container_frame_ = nullptr;
  QVBoxLayout* msg_layout_ = nullptr;
  QLabel* msg_head_label_ = nullptr;

  AdvancedValidateStates validate_states_;

  AdvancedPartitionButton* hovered_part_button_ = nullptr;

  // To hold all error labels.
  QVector<AdvancedPartitionErrorLabel*> error_labels_;

 private slots:
  // Clear error message list and hide message container.
  void clearErrorMessages();

  void onDeletePartitionTriggered(const Partition::Ptr partition);
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);

  // Handles editPartitionTriggered() signal from advanced partition button.
  void onEditPartitionTriggered(const Partition::Ptr partition);

  // Change alpha value of partition button when mouse enters error label.
  void onErrorLabelEntered();

  // Reset alpha value of partition button when mouse moves out of error label.
  void onErrorLabelLeaved();

  // Handles newPartitionTriggered() signal from advanced partition button.
  void onNewPartitionTriggered(const Partition::Ptr partition);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
