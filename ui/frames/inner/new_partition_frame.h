// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H

#include <QFrame>
class QLabel;

#include "partman/partition.h"

namespace installer {

class CommentLabel;
class FsModel;
class MountPointModel;
class NavButton;
class PartitionDelegate;
class PartitionSizeSlider;
class TableComboBox;
class TitleLabel;

class NewPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  NewPartitionFrame(PartitionDelegate* delegate_, QWidget* parent = nullptr);

  // Reset partition information at |partition_path|.
  void setPartition(const Partition& partition);

 signals:
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  QLabel* type_label_ = nullptr;
  TableComboBox* type_box_ = nullptr;
  QLabel* alignment_label_ = nullptr;
  TableComboBox* alignment_box_ = nullptr;
  QLabel* fs_label_ = nullptr;
  TableComboBox* fs_box_ = nullptr;
  QLabel* mount_point_label_ = nullptr;
  TableComboBox* mount_point_box_ = nullptr;
  QLabel* size_label_ = nullptr;
  PartitionSizeSlider* size_slider_ = nullptr;

  NavButton* cancel_button_ = nullptr;
  NavButton* create_button_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;
  MountPointModel* mount_point_model_ = nullptr;
  Partition partition_;

 private slots:
  void onCreateButtonClicked();

  // Hide mount_point_box_ when specific fs is selected
  void onFsChanged(int index);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
