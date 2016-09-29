// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_table_warning_frame.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/expanded_nav_button.h"
#include "ui/widgets/frosted_frame.h"
#include "ui/widgets/list_label.h"
#include "ui/widgets/list_title_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

PartitionTableWarningFrame::PartitionTableWarningFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName(QStringLiteral("partition_table_warning_frame"));

  this->initUI();
  this->initConnections();
}

void PartitionTableWarningFrame::initConnections() {
  connect(reject_button_, &QPushButton::clicked,
          this, &PartitionTableWarningFrame::declined);
  connect(accept_button_, &QPushButton::clicked,
          this, &PartitionTableWarningFrame::accepted);
}

void PartitionTableWarningFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Warning"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("It is detected that your motherboard boot loader is EFI and "
         "the disk format is MBR, thus the system can not be installed "
         "directly; to continue, please select one of the solutions below."));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  ListTitleLabel* left_label = new ListTitleLabel(
      QString("A.") + tr("Disable UEFI"));
  ListLabel* list_label1 = new ListLabel(
      QString("1.") + tr("Reboot, enter BIOS, and disable UEFI"));
  ListLabel* list_label2 = new ListLabel(
      QString("2.") +
      tr("Exit BIOS, and enter deepin installation"));
  ListTitleLabel* left_label2 = new ListTitleLabel(
      QString("B.") + tr("Format the disk"));
  ListLabel* list_label3 = new ListLabel(
      QString("1.") +
      tr("Please make backup for all your data to avoid data loss"));
  ListLabel* list_label4 = new ListLabel(
      QString("2.") +
      tr("Please double check if you have made backup for all the data, "
         "and reboot to enter this interface"));
  QVBoxLayout* left_frame_layout = new QVBoxLayout();
  left_frame_layout->setContentsMargins(20, 20, 20, 20);
  left_frame_layout->setSpacing(kMainLayoutSpacing);
  left_frame_layout->addWidget(left_label);
  left_frame_layout->addWidget(list_label1);
  left_frame_layout->addWidget(list_label2);
  left_frame_layout->addSpacing(10);
  left_frame_layout->addWidget(left_label2);
  left_frame_layout->addWidget(list_label3);
  left_frame_layout->addWidget(list_label4);
  left_frame_layout->addStretch();
  FrostedFrame* left_frame = new FrostedFrame();
  left_frame->setLayout(left_frame_layout);

  ListTitleLabel* right_label = new ListTitleLabel(tr("Continue"));
  ListLabel* list_label5 = new ListLabel(
      QString("1.") +
      tr("Please make sure you have made backup for all the data, "
         "then continue"));
  ListLabel* list_label6 = new ListLabel(
      QString("2.") + tr("Continue installation will format your disk"));
  QVBoxLayout* right_frame_layout = new QVBoxLayout();
  right_frame_layout->setSpacing(kMainLayoutSpacing);
  right_frame_layout->setContentsMargins(20, 20, 20, 20);
  right_frame_layout->addWidget(right_label);
  right_frame_layout->addWidget(list_label5);
  right_frame_layout->addWidget(list_label6);
  right_frame_layout->addStretch();
  FrostedFrame* right_frame = new FrostedFrame();
  right_frame->setLayout(right_frame_layout);

  reject_button_ = new ExpandedNavButton(tr("Reboot"));
  accept_button_ = new ExpandedNavButton(tr("Continue"));

  QGridLayout* content_layout = new QGridLayout();
  content_layout->setAlignment(Qt::AlignCenter);
  content_layout->setVerticalSpacing(kMainLayoutSpacing);
  content_layout->setHorizontalSpacing(40);
  content_layout->setColumnMinimumWidth(0, 480);
  content_layout->setColumnMinimumWidth(1, 480);
  content_layout->addWidget(left_frame, 0, 0);
  content_layout->addWidget(right_frame, 0, 1);
  content_layout->addWidget(reject_button_, 1, 0);
  content_layout->addWidget(accept_button_, 1, 1);

  QFrame* content_wrapper = new QFrame();
  content_wrapper->setLayout(content_layout);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addSpacing(50);
  layout->setAlignment(Qt::AlignCenter);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(content_wrapper);
  layout->addStretch();

  this->setLayout(layout);
}

}  // namespace installer