// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H

#include <QFrame>

#include "ui/frames/delegates/partition_delegate.h"

namespace ui {

class FlatButton;

class AdvancedPartitionItem : public QFrame {
  Q_OBJECT

 public:
  AdvancedPartitionItem(const PartitionWrap& partition,
                        QWidget* parent = nullptr);

  enum class EditDeleteButtonState {
    Hide,  // hide edit-delete-button
    Edit,  // displays as edit-partition-button
    Delete,  // displays as delete-partition-button
  };

  // Update edit-delete-button state.
  void setEditDeleteButtonState(EditDeleteButtonState state);

 signals:
  // Emitted when delete partition button is clicked.
  void deletePartitionTriggered();

  // Emitted when edition partition partition is clicked.
  void editPartitionTriggered();

 private:
  void initConnections();
  void initUI();

  const PartitionWrap& partition_;

  FlatButton* edit_delete_button_ = nullptr;
  EditDeleteButtonState edit_delete_button_state_;

 private slots:
  void onEditDeleteButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
