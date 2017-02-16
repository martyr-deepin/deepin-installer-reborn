// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class SpinnerLabel;
class TitleLabel;

// Display loading animation when creating new partition table.
class NewTableLoadingFrame : public QFrame {
  Q_OBJECT

 public:
  explicit NewTableLoadingFrame(QWidget* parent = nullptr);

 protected:
  void changeEvent(QEvent* event) override;

  // Stop spinner animation.
  void hideEvent(QHideEvent* event) override;

  // Start spinner animation.
  void showEvent(QShowEvent* event) override;

 private:
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  SpinnerLabel* spinner_label_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_TABLE_LOADING_FRAME_H
