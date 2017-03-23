// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
#include <QVector>
#include <ui/widgets/advanced_partition_error_label.h>

class QButtonGroup;
class QLabel;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

#include "partman/partition.h"
#include "ui/delegates/advanced_partition_delegate.h"

namespace installer {

class AdvancedPartitionAnimations;
class AdvancedPartitionButton;
class AdvancedPartitonErrorLabel;

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

 signals:
  void requestEditPartitionFrame(const Partition& partition);
  void requestNewPartitionFrame(const Partition& partition);

  // Emitted when partition table not appropriate at |device_path|.
  void requestNewTable(const QString& device_path);

  // Show PartitionNumberLimitationFrame.
  void requestPartitionNumberLimitationFrame();

  // Show SelectBootloaderFrame.
  void requestSelectBootloaderFrame();

 public slots:
  // Update states when EditPartitionFrame returns.
  void onEditPartitionFrameFinished();

  // Update states when NewPartitionFrame returns.
  void onNewPartitionFrameFinished();

  // Update bootloader path in button.
  // This slots is connected to SelectBootloaderFrame.
  void setBootloaderPath(const QString& bootloader_path);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();
  void repaintDevices();

  // Scroll to top of content area.
  void scrollContentToTop();

  void hideErrorMessage(AdvancedValidateState state);
  void hideErrorMessages();
  void showErrorMessage(AdvancedValidateState state, bool enable_animation);
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

  // TODO(xushaohua): Remove this property.
  AdvancedValidateStates validate_states_;

  AdvancedPartitionAnimations* animations_ = nullptr;

  AdvancedPartitonErrorLabel* hovered_error_label_ = nullptr;
  AdvancedPartitionButton* hovered_part_button_ = nullptr;

  // To hold all error labels.
  QVector<AdvancedPartitionErrorLabel*> error_labels_;

 private slots:
  // Clear error message list and hide message container.
  void clearErrorMessages();

  void onDeletePartitionTriggered(const Partition& partition);
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);

  // Handles editPartitionTriggered() signal from advanced partition button.
  void onEditPartitionTriggered(const Partition& partition);

  // Change alpha value of partition button when mouse enters error label.
  void onErrorLabelEntered();

  // Reset alpha value of partition button when mouse moves out of error label.
  void onErrorLabelLeaved();

  // Handles newPartitionTriggered() signal from advanced partition button.
  void onNewPartitionTriggered(const Partition& partition);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
