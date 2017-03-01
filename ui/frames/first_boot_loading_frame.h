// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_FIRST_BOOT_LOADING_FRAME_H
#define INSTALLER_UI_FRAMES_FIRST_BOOT_LOADING_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class SpinnerLabel;
class TitleLabel;

// Display loading animation after SystemInfoFrame in first-boot-setup window.
class FirstBootLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit FirstBootLoadingFrame(QWidget* parent = nullptr);

 protected:
  void changeEvent(QEvent* event) override;

  // Stop spinner animation.
  void hideEvent(QHideEvent* event) override;

  // Start spinner animation.
  void showEvent(QShowEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  SpinnerLabel* spinner_label_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_FIRST_BOOT_LOADING_FRAME_H
