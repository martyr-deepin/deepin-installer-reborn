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

#ifndef INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
#define INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H

#include "ui/widgets/pointer_button.h"

#include "partman/partition.h"

namespace installer {

// Display partition information in AdvancedPartitionFrame.
class AdvancedPartitionButton : public PointerButton {
  Q_OBJECT

 public:
  explicit AdvancedPartitionButton(const Partition::Ptr partition,
                                   QWidget* parent = nullptr);

  // Get partition struct binding to current button.
  const Partition::Ptr partition() const;

 signals:
  // Emitted when delete partition button is clicked.
  void deletePartitionTriggered(const Partition::Ptr partition);

  // Emitted when edit-partition button is clicked.
  void editPartitionTriggered(const Partition::Ptr partition);

  // Emitted when edit-partition button is clicked and type of current
  // partition is Unallocated.
  void newPartitionTriggered(const Partition::Ptr partition);

 public slots:
  void setEditable(bool editable);

 private:
  void initConnections();
  void initUI();

  const Partition::Ptr partition_;

  // Switch status between new-partition, edit-partition and delete-partition.
  PointerButton* control_button_ = nullptr;
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
