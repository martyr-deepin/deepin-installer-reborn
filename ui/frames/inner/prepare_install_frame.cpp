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
  CommentLabel* comment_label = new CommentLabel(
      tr("Please backup important data and confirm the following operations"));
  SubheadLabel* subhead_label = new SubheadLabel(
      tr("The following operations will be executed, please confirm and "
         "continue to avoid data loss"));
  desc_label_ = new QLabel();
  abort_button_ = new NavButton(tr("Back"));
  continue_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addWidget(comment_label, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(subhead_label, 0, Qt::AlignCenter);
  layout->addWidget(desc_label_, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer