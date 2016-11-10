// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

#include <QHBoxLayout>
#include <QtCore/QEvent>

#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/models/partition_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Select location for boot loader";
const char kTextComment[] = "If you do not understand the detailed settings, "
                            "please select default settings";
const char kTextBack[] = "Back";

}  // namespace

SelectBootloaderFrame::SelectBootloaderFrame(PartitionDelegate* delegate,
                                             QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("select_bootloader_frame");

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::selectRecommendedBootloader() {
  // First read the first device path from delegate.
  // Then select it in list view
  // Thus, both AdvancedPartitionFrame and PartitionDelegate will
  // update bootloader path
  // TODO(xushaohua): read device list
}

void SelectBootloaderFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    back_button_->setText(tr(kTextBack));
  } else {
    QFrame::changeEvent(event);
  }
}

void SelectBootloaderFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SelectBootloaderFrame::finished);
  connect(list_view_->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &SelectBootloaderFrame::onPartitionListViewSelected);
}

void SelectBootloaderFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  list_view_ = new FramelessListView();
  list_model_ = new PartitionListModel(delegate_, this);
  list_view_->setModel(list_model_);

  back_button_ = new NavButton(tr(kTextBack));

  QVBoxLayout* layout = new QVBoxLayout();
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
}

void SelectBootloaderFrame::onPartitionListViewSelected(
    const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  if (current.isValid()) {
    // TODO(xushaohua):
    emit this->bootloaderUpdated("/dev/sda");
  }
}

}  // namespace installer