// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
#define INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H

#include "ui/widgets/flat_button.h"
class QLabel;

#include "partman/partition.h"

namespace installer {

class AdvancedPartitionItem : public FlatButton {
  Q_OBJECT

 public:
  AdvancedPartitionItem(const Partition& partition,
                        QWidget* parent = nullptr);

  void setEditable(bool editable);

 signals:
  // Emitted when delete partition button is clicked.
  void deletePartitionTriggered(const Partition& partition);

  // Emitted when edit-partition button is clicked.
  void editPartitionTriggered(const Partition& partition);

  // Emitted when edit-partition button is clicked and type of current
  // partition is Unallocated.
  void newPartitionTriggered(const Partition& partition);

 private:
  void initConnections();
  void initUI();

  const Partition& partition_;

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

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
