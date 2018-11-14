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
class AdvancedPartitionDelegate;
class PartitionTypeModel;
class PartitionSizeSlider;
class TableComboBox;
class TitleLabel;

class NewPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  NewPartitionFrame(AdvancedPartitionDelegate* delegate,
                    QWidget* parent = nullptr);

  // Reset partition information at |partition_path|.
  void setPartition(const Partition::Ptr partition);

 signals:
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Update current slider size based on mount point and filesystem type.
  void updateSlideSize();

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

  AdvancedPartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;
  MountPointModel* mount_point_model_ = nullptr;
  PartitionTypeModel* type_model_ = nullptr;
  Partition::Ptr partition_;

  // To memorize slider value.
  qint64 last_slider_value_;

 private slots:
  // Append operations to |delegate| when create_button_ is clicked.
  void onCreateButtonClicked();

  // Hide mount_point_box_ when specific fs is selected
  void onFsChanged(int index);

  // Change value of size-slider when mount point is special.
  void onMountPointChanged(int index);

  // Update last_slider_value_ when it is updated.
  void onSizeSliderValueChanged(qint64 size);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
