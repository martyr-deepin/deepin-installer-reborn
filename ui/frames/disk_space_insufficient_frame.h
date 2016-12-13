// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H
#define INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class NavButton;
class TitleLabel;

// This frame is displayed when no enough disk space is detected.
class DiskSpaceInsufficientFrame : public QFrame {
  Q_OBJECT

 public:
  explicit DiskSpaceInsufficientFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when abort-button is clicked.
  void finished();

 protected:
  void changeEvent(QEvent* event);

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* abort_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_DISK_SPACE_INSUFFICIENT_FRAME_H
