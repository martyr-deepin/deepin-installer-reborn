// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H

#include <QFrame>
class QCheckBox;
class QLabel;
class QProgressBar;

#include "partman/partition.h"

namespace installer {

class FsModel;
class MountPointModel;
class NavButton;
class PartitionDelegate;
class TableComboBox;

class EditPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  EditPartitionFrame(PartitionDelegate* delegate, QWidget* parent = nullptr);

  // Set partition information at |partition_path|.
  void setPartition(const Partition& partition);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  QLabel* os_label_ = nullptr;
  QLabel* name_label_ = nullptr;
  QLabel* usage_label_ = nullptr;
  QProgressBar* usage_bar_ = nullptr;
  TableComboBox* fs_box_ = nullptr;
  TableComboBox* mount_point_box_ = nullptr;
  QLabel* mount_point_label_ = nullptr;
  QCheckBox* format_check_box_ = nullptr;
  QLabel* format_label_ = nullptr;
  NavButton* cancel_button_ = nullptr;
  NavButton* ok_button_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;
  MountPointModel* mount_point_model_ = nullptr;
  Partition partition_;

 private slots:
  void onFsChanged(int index);
  void onOkButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_EDIT_PARTITION_FRAME_H
