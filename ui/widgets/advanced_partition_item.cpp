// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_item.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/widgets/flat_button.h"

namespace ui {

AdvancedPartitionItem::AdvancedPartitionItem(const PartitionWrap& partition,
                                             QWidget* parent)
    : QFrame(parent),
      partition_(partition),
      edit_delete_button_state_(EditDeleteButtonState::Hide) {
  this->setObjectName(QStringLiteral("advanced_partition_item"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionItem::setEditDeleteButtonState(
    EditDeleteButtonState state) {
  switch (state) {
    case EditDeleteButtonState::Delete: {
      edit_delete_button_->setIcon(QIcon(":/images/delete_partition.png"));
      edit_delete_button_->show();
      break;
    }
    case EditDeleteButtonState::Edit: {
      edit_delete_button_->setIcon(QIcon(":/images/edit_partition.png"));
      edit_delete_button_->show();
      break;
    }
    case EditDeleteButtonState::Hide: {
      edit_delete_button_->hide();
    }
  }
  edit_delete_button_state_ = state;
}

void AdvancedPartitionItem::initConnections() {
  connect(edit_delete_button_, &QPushButton::clicked,
          this, &AdvancedPartitionItem::onEditDeleteButtonClicked);
}

void AdvancedPartitionItem::initUI() {
  // os-prober logo
  // filesystem type
  // partition label
  // partition space usage
  // mount point
  // tip
  // filesystem name
  QLabel* fs_name = new QLabel("ext4");
  // edit/delete button
  edit_delete_button_ = new FlatButton();
  edit_delete_button_->setFixedSize(18, 18);
  this->setEditDeleteButtonState(EditDeleteButtonState::Delete);

  // TODO(xuhaohua): Use fixed layout instead.
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(fs_name);
  layout->addWidget(edit_delete_button_);

  this->setLayout(layout);

  this->setFixedSize(480, 36);
}

void AdvancedPartitionItem::onEditDeleteButtonClicked() {
  switch (edit_delete_button_state_) {
    case EditDeleteButtonState::Delete: {
      emit this->deletePartitionTriggered();
      break;
    }
    case EditDeleteButtonState::Edit: {
      emit this->editPartitionTriggered();
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace ui