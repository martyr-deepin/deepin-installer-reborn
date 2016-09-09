// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H

#include <QFrame>
#include <ui/widgets/table_combo_box.h>

namespace ui {

class FsModel;
class NavButton;
class TableComboBox;
class PartitionDelegate;

class NewPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  NewPartitionFrame(PartitionDelegate* delegate_, QWidget* parent = nullptr);

  // Reset partition information at |partition_path|.
  void setPath(const QString& partition_path);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  TableComboBox* type_box_ = nullptr;
  TableComboBox* location_box_ = nullptr;
  TableComboBox* fs_box_ = nullptr;
  TableComboBox* mount_point_box_ = nullptr;
  TableComboBox* size_box_ = nullptr;

  NavButton* cancel_button_ = nullptr;
  NavButton* create_button_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;
  FsModel* fs_model_ = nullptr;

 private slots:
  void onCreateButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
