// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H

#include <QFrame>
#include <ui/widgets/table_combo_box.h>

namespace ui {

class NavButton;
class TableComboBox;

class NewPartitionFrame : public QFrame {
  Q_OBJECT

 public:
  explicit NewPartitionFrame(QWidget* parent = nullptr);

  void reset();

 signals:
  // Emitted when aborting creating new partition.
  void canceled();

  // Emitted when a new partition is created.
  void created();

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
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_NEW_PARTITION_FRAME_H
