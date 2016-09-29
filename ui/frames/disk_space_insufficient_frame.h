// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H
#define INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H

#include <QFrame>

namespace installer {

// This frame is displayed when no enough disk space is detected.
class DiskSpaceInsufficientFrame : public QFrame {
  Q_OBJECT

 public:
  explicit DiskSpaceInsufficientFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when abort-button is clicked.
  void finished();

 private:
  void initUI();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H
