/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H

#include <QFrame>
class QCheckBox;
class QLabel;
class QProgressBar;

#include "partman/partition.h"

namespace installer {

class AdvancedPartitionDelegate;
class CommentLabel;
class FsModel;
class MountPointModel;
class NavButton;
class TableComboBox;
class TitleLabel;

class EditPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  EditPartitionFrame(AdvancedPartitionDelegate* delegate,
                     QWidget* parent = nullptr);

  // Set partition information at |partition_path|.
  void setPartition(const Partition::Ptr partition);

 signals:
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  // Format partition forcefully if |force| is true.
  void forceFormat(bool force);

  // Set check state of format_box based on current mount-point and fs-type.
  void updateFormatBoxState();

  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  QLabel* os_label_ = nullptr;
  QLabel* name_label_ = nullptr;
  QLabel* usage_label_ = nullptr;
  QProgressBar* usage_bar_ = nullptr;
  TableComboBox* fs_box_ = nullptr;
  QLabel* fs_label_ = nullptr;
  TableComboBox* mount_point_box_ = nullptr;
  QLabel* mount_point_label_ = nullptr;
  QCheckBox* format_check_box_ = nullptr;
  QLabel* format_label_ = nullptr;
  NavButton* cancel_button_ = nullptr;
  NavButton* ok_button_ = nullptr;

  AdvancedPartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;
  MountPointModel* mount_point_model_ = nullptr;
  Partition::Ptr partition_;

 private slots:
  // Hide mount_point_box_ when specific fs is selected
  void onFsChanged(int index);

  // Update format box state when mount-point is root.
  void onMountPointChanged(int index);

  void onOkButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H
