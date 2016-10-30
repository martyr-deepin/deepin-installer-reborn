// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

#include <QHBoxLayout>

#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/models/partition_list_model.h"
#include "ui/views/frameless_list_view.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

SelectBootloaderFrame::SelectBootloaderFrame(PartitionDelegate* delegate,
                                             QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName(QStringLiteral("select_bootloader_frame"));

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::updatePartitionList() {
  list_model_->updatePartitionList();
}

void SelectBootloaderFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SelectBootloaderFrame::finished);
}

void SelectBootloaderFrame::initUI() {
  TitleLabel* title_label =
      new TitleLabel(tr("Select location for boot loader"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("If you do not understand the detailed settings, "
         "please select default settings"));

  list_view_ = new FramelessListView();
  list_model_ = new PartitionListModel(delegate_, this);
  list_view_->setModel(list_model_);

  back_button_ = new NavButton(tr("Back"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addSpacing(40);
  layout->addWidget(list_view_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addSpacing(50);
  layout->addWidget(back_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer