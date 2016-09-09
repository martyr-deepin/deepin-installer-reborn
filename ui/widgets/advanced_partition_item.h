// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H

#include "ui/widgets/flat_button.h"
class QLabel;

#include "ui/frames/delegates/partition_delegate.h"

namespace ui {

class AdvancedPartitionItem : public FlatButton {
  Q_OBJECT

 public:
  AdvancedPartitionItem(const partman::Partition& partition,
                        QWidget* parent = nullptr);

  void setEditable(bool editable);

 signals:
  // Emitted when delete partition button is clicked.
  void deletePartitionTriggered(const QString& partition_path);

  // Emitted when edit-partition button is clicked.
  void editPartitionTriggered(const partman::Partition& partition);

  // Emitted when edit-partition button is clicked and type of current
  // partition is Unallocated.
  void newPartitionTriggered(const partman::Partition& partition);

 private:
  void initConnections();
  void initUI();

  const partman::Partition& partition_;

  QLabel* partition_label_ = nullptr;
  QLabel* partition_path_label_ = nullptr;
  QLabel* usage_label_ = nullptr;
  QLabel* mount_point_label_ = nullptr;
  QLabel* tip_label_ = nullptr;
  QLabel* fs_type_label_ = nullptr;

  // Switch status between new-partition, edit-partition and delete-partition.
  FlatButton* control_button_ = nullptr;
  enum class ControlStatus {
    Delete,
    Edit,
    Hide,
    New,
  };
  ControlStatus control_status_;
  bool editable_;
  void updateStatus();

 private slots:
  void onControlButtonClicked();

  // Handle toggled signal from self.
  void onToggled();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
