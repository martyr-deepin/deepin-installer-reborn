// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H

#include <QFrame>
class QSlider;

#include "partman/partition.h"

namespace installer {

class FsModel;
class MountPointModel;
class NavButton;
class TableComboBox;
class PartitionDelegate;

class NewPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  NewPartitionFrame(PartitionDelegate* delegate_, QWidget* parent = nullptr);

  // Reset partition information at |partition_path|.
  void setPartition(const Partition& partition);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  TableComboBox* type_box_ = nullptr;
  TableComboBox* alignment_box_ = nullptr;
  TableComboBox* fs_box_ = nullptr;
  TableComboBox* mount_point_box_ = nullptr;
  QSlider* size_slider_ = nullptr;

  NavButton* cancel_button_ = nullptr;
  NavButton* create_button_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;
  MountPointModel* mount_point_model_ = nullptr;
  Partition partition_;

 private slots:
  void onCreateButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
