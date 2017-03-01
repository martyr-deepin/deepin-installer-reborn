// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

#include <QEvent>
#include <QHBoxLayout>

#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/models/bootloader_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

SelectBootloaderFrame::SelectBootloaderFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("select_bootloader_frame");

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Select location for boot loader"));
    comment_label_->setText(
        tr("If you do not understand the detailed settings, "
           "please select default settings"));
    back_button_->setText(tr("Back"));
  } else {
    QFrame::changeEvent(event);
  }
}

void SelectBootloaderFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SelectBootloaderFrame::finished);
  connect(list_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &SelectBootloaderFrame::onPartitionListViewSelected);
  connect(list_model_, &BootloaderListModel::rowChanged,
          this, &SelectBootloaderFrame::onModelChanged);

  connect(this, &SelectBootloaderFrame::deviceRefreshed,
          list_model_, &BootloaderListModel::onDeviceRefreshed);
}

void SelectBootloaderFrame::initUI() {
  title_label_ = new TitleLabel(tr("Select location for boot loader"));
  comment_label_ = new CommentLabel(
      tr("If you do not understand the detailed settings, "
         "please select default settings"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  list_view_ = new FramelessListView();
  list_view_->setFixedWidth(560);
  list_model_ = new BootloaderListModel(this);
  list_view_->setModel(list_model_);

  back_button_ = new NavButton(tr("Back"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addSpacing(40);
  layout->addWidget(list_view_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addSpacing(50);
  layout->addWidget(back_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

void SelectBootloaderFrame::onModelChanged() {
  // Select recommended bootloader.
  const QModelIndex index = list_model_->getRecommendedIndex();
  if (index.isValid()) {
    list_view_->selectionModel()->select(index, QItemSelectionModel::Select);
    // Update selection model explicitly.
    emit list_view_->selectionModel()->currentChanged(index, QModelIndex());
  }
}

void SelectBootloaderFrame::onPartitionListViewSelected(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  if (current.isValid()) {
    const QString path = list_model_->getPath(current);
    if (!path.isEmpty()) {
      // Both AdvancedPartitionFrame and PartitionDelegate will
      // update advanced bootloader path
      emit this->bootloaderUpdated(path, false);
    }
  }
}

}  // namespace installer