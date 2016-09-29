// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H

#include <QFrame>

namespace installer {

// Displays loading page when partition manager is scanning disk devices.
class PartitionLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionLoadingFrame(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_PARTITION_LOADING_FRAME_H
