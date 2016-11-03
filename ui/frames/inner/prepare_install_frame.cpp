// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/prepare_install_frame.h"

#include <QLabel>

#include "base/file_util.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PrepareInstallFrame::PrepareInstallFrame(PartitionDelegate* delegate,
                                         QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName(QStringLiteral("prepare_install_frame"));

  this->initUI();
  this->initConnections();
}

void PrepareInstallFrame::updateDescription() {
  const QStringList desc_list(delegate_->getOperationDescriptions());
  const QString prefix("•   ");
  QStringList modified_desc_list;
  for (const QString& desc_item : desc_list) {
    modified_desc_list.append(prefix + desc_item);
  }
  // NOTE(xushaohua): Add tow line breaks to simulate line-height property
  // in QLabel.
  const QString description = modified_desc_list.join("\n\n");
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
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Please backup important data and confirm the following operations"));
  QLabel* subtitle_label = new QLabel(
      tr("The following operations will be executed, please confirm and "
         "continue to avoid data loss"));
  subtitle_label->setObjectName("subtitle_label");

  desc_label_ = new QLabel();
  desc_label_->setObjectName("desc_label");
  desc_label_->setWordWrap(true);
  // TODO(xushaohua): Add a ScrollArea.

  abort_button_ = new NavButton(tr("Back"));
  continue_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(subtitle_label, 0, Qt::AlignCenter);
  layout->addWidget(desc_label_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/prepare_install_frame.css"));
}

}  // namespace installer