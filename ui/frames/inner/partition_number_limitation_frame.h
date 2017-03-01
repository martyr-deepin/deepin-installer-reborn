// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_PARTITION_NUMBER_LIMITATION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_PARTITION_NUMBER_LIMITATION_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class NavButton;
class TitleLabel;

// Display this info page when reaching max number of primary partitions
// and no more partitions can be created anymore.
class PartitionNumberLimitationFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionNumberLimitationFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when back button is clicked.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment1_label_ = nullptr;
  CommentLabel* comment2_label_ = nullptr;
  NavButton* back_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_PARTITION_NUMBER_LIMITATION_FRAME_H
