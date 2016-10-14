// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/prepare_install_frame.h"

#include <QHBoxLayout>

#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/subhead_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

PrepareInstallFrame::PrepareInstallFrame(PartitionDelegate* delegate,
                                         QWidget* parent)
    : QFrame(parent), delegate_(delegate) {
  this->setObjectName(QStringLiteral("prepare_install_frame"));

  this->initUI();
  this->initConnections();
}

void PrepareInstallFrame::updateDescription() {
  const QStringList desc_list = delegate_->getOperationDescriptions();
  const QString description = desc_list.join(QChar('\n'));
  qDebug() << "operation descriptions:" << description;
  desc_label_->setText(description);
}

void PrepareInstallFrame::initConnections() {
  connect(abort_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::aborted);
  connect(continue_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::finished);
}

void PrepareInstallFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Prepare for Installation"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Please backup important data and confirm the following operations"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  SubheadLabel* subhead_label = new SubheadLabel(
      tr("The following operations will be executed, please confirm and "
         "continue to avoid data loss"));
  QHBoxLayout* subhead_layout = new QHBoxLayout();
  subhead_layout->addWidget(subhead_label);

  desc_label_ = new QLabel();

  abort_button_ = new NavButton(tr("Back"));
  QHBoxLayout* abort_button_layout = new QHBoxLayout();
  abort_button_layout->addWidget(abort_button_);

  continue_button_ = new NavButton(tr("Continue"));
  QHBoxLayout* continue_button_layout = new QHBoxLayout();
  continue_button_layout->addWidget(continue_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->setAlignment(Qt::AlignCenter);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(subhead_layout);
  layout->addWidget(desc_label_);
  layout->addStretch();
  layout->addLayout(abort_button_layout);
  layout->addLayout(continue_button_layout);

  this->setLayout(layout);
}

}  // namespace installer