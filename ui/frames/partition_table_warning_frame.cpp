// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_table_warning_frame.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/expanded_nav_button.h"
#include "ui/widgets/frosted_frame.h"
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
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("It is detected that your motherboard boot loader is EFI and "
         "the disk format is MBR, thus the system can not be installed "
         "directly; to continue, please select one of the solutions below."));

  QLabel* list_title1 = new QLabel(QString("A.") + tr("Disable UEFI"), this);
  list_title1->setObjectName("list_title1");
  QLabel* list_item1 = new QLabel(
      tr("1.Reboot, enter BIOS, and disable UEFI") + "\n" +
      tr("2.Exit BIOS, and enter deepin installation"),
      this);
  list_item1->setObjectName("list_item1");
  list_item1->setWordWrap(true);
  QLabel* list_title2 = new QLabel(QString("B.") + tr("Format the disk"), this);
  list_title2->setObjectName("list_title2");
  QLabel* list_item2 = new QLabel(
      tr("1.Please make backup for all your data to avoid data loss") + "\n" +
      tr("2.Please double check if you have made backup for all the data, "
         "and reboot to enter this interface, Please double check if you have"),
      this);
  list_item2->setObjectName("list_item2");
  list_item2->setWordWrap(true);
  QVBoxLayout* left_frame_layout = new QVBoxLayout();
  left_frame_layout->setContentsMargins(20, 20, 20, 20);
  left_frame_layout->setSpacing(0);
  left_frame_layout->addWidget(list_title1);
  left_frame_layout->addWidget(list_item1);
  left_frame_layout->addStretch();
  left_frame_layout->addWidget(list_title2);
  left_frame_layout->addWidget(list_item2);
  left_frame_layout->addStretch();
  FrostedFrame* left_frame = new FrostedFrame();
  left_frame->setFixedWidth(480);
  left_frame->setLayout(left_frame_layout);

  QLabel* list_title3 = new QLabel(tr("Continue"), this);
  list_title3->setObjectName("list_title3");
  QLabel* list_item3 = new QLabel(
      tr("1.Please make sure you have made backup for all the data, "
         "then continue") + "\n" +
      tr("2.Continue installation will format your disk"),
      this);
  list_item3->setObjectName("list_item3");
  list_item3->setWordWrap(true);
  QVBoxLayout* right_frame_layout = new QVBoxLayout();
  right_frame_layout->setContentsMargins(20, 20, 20, 20);
  right_frame_layout->setSpacing(0);
  right_frame_layout->addWidget(list_title3);
  right_frame_layout->addWidget(list_item3);
  right_frame_layout->addStretch();
  FrostedFrame* right_frame = new FrostedFrame();
  right_frame->setFixedWidth(480);
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

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(content_layout);
  layout->addStretch();

  this->setLayout(layout);
  this->setStyleSheet(
      ReadTextFileContent(":/styles/partition_table_warning_frame.css"));
}

}  // namespace installer