// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H

#include <QFrame>
class QButtonGroup;
class QLabel;
class QPushButton;
class QVBoxLayout;

#include "partman/partition.h"

namespace installer {

class PartitionDelegate;

// Advanced partition mode
class AdvancedPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionFrame(PartitionDelegate* delegate_,
                         QWidget* parent = nullptr);

  // Check whether partition operations are appropriate:
  //  * / partition is set and large enough;
  //  * An EFI partition is created if EFI mode is on;
  //  * A linux-swap partition is set;
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
  // Update bootloader path in button.
  // This slots is connected to SelectBootloaderFrame.
  void setBootloaderPath(const QString& bootloader_path);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();
  void repaintDevices();

  PartitionDelegate* delegate_ = nullptr;

  QPushButton* bootloader_tip_button_ = nullptr;
  QButtonGroup* partition_button_group_ = nullptr;
  QVBoxLayout* partition_layout_ = nullptr;
  QPushButton* bootloader_button_ = nullptr;
  QPushButton* editing_button_ = nullptr;

  QFrame* msg_container_frame_ = nullptr;
  QVBoxLayout* msg_layout_ = nullptr;
  QLabel* msg_head_label_ = nullptr;
  enum class ErrorMessageType {
    BootTooSmall,
    EfiMissing,
    EfiTooSmall,
    RootMissing,
    RootTooSmall,
    // TODO(xushaohua): Loongson boot position.
  };
  struct ErrorMessage {
    QString text;
    ErrorMessageType type;
  };
  typedef QVector<ErrorMessage> ErrorMessageList;
  ErrorMessageList error_messages_;

  // Add a new error message to list.
  // And show error message container.
  void addErrorMessage(const QString& text, ErrorMessageType type);

 private slots:
  // Clear error message list and hide message container.
  void clearErrorMessages();

  void onDeletePartitionTriggered(const Partition& partition);
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);

  // Handles editPartitionTriggered() signal from advanced partition button.
  void onEditPartitionTriggered(const Partition& partition);

  // Handles newPartitionTriggered() signal from advanced partition button.
  void onNewPartitionTriggered(const Partition& partition);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
