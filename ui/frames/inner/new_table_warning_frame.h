// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H_
#define INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H_

#include <QFrame>

namespace installer {

class CommentLabel;
class NavButton;
class TitleLabel;

// Display warning message before creating new partition table.
class NewTableWarningFrame : public QFrame {
  Q_OBJECT

 public:
  explicit NewTableWarningFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when cancel_button_ is clicked.
  void canceled();

  // Emitted when confirm_button_ is clicked.
  void confirmed();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* cancel_button_ = nullptr;
  NavButton* confirm_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H_
