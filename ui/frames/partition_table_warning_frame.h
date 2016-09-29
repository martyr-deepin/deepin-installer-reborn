// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PARTITION_TABLE_WARNING_FRAME_H
#define INSTALLER_UI_FRAMES_PARTITION_TABLE_WARNING_FRAME_H

#include <QFrame>

namespace installer {

class ExpandedNavButton;

// Displayed when partition table type and system boot method does not match.
class PartitionTableWarningFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionTableWarningFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when reboot button is clicked.
  void declined();

  // Emitted when continue button is clicked.
  void accepted();

 private:
  void initConnections();
  void initUI();

  ExpandedNavButton* reject_button_ = nullptr;
  ExpandedNavButton* accept_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PARTITION_TABLE_WARNING_FRAME_H
