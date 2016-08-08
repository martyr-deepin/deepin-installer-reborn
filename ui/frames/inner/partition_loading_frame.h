// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H

#include <QFrame>

namespace ui {

// Displays loading page when partition manager is scanning disk devices.
class PartitionLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionLoadingFrame(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
