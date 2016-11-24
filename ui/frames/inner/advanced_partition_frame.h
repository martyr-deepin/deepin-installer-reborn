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

  QLabel* bootloader_label_ = nullptr;
  QButtonGroup* partition_button_group_ = nullptr;
  QVBoxLayout* partition_layout_ = nullptr;
  QPushButton* bootloader_button_ = nullptr;
  QPushButton* editing_button_ = nullptr;
  // Display message at bottom of page.
  // Its content is set in validate() and is cleared when NewPartitionFrame or
  // EditPartitionFrame is raised.
  QLabel* msg_label_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onEditButtonToggled(bool toggle);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_ADVANCED_PARTITION_FRAME_H
