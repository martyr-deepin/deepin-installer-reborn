// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H

#include <QFrame>
class QLabel;

namespace installer {

class CommentLabel;
class NavButton;
class PartitionDelegate;
class TitleLabel;

class PrepareInstallFrame : public QFrame {
  Q_OBJECT

 public:
  PrepareInstallFrame(PartitionDelegate* delegate, QWidget* parent = nullptr);

  // Update descriptions of operations.
  void updateDescription();

 signals:
  // Emitted when abort-button is clicked, returning to previous page.
  void aborted();

  // Emitted when continue-button is clicked. Start actual installation process.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  QLabel* subtitle_label_ = nullptr;
  NavButton* abort_button_ = nullptr;
  NavButton* continue_button_ = nullptr;
  QLabel* description_label_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H
