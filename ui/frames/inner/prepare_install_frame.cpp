// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/prepare_install_frame.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

#include "base/file_util.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PrepareInstallFrame::PrepareInstallFrame(PartitionDelegate* delegate,
                                         QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("prepare_install_frame");

  this->initUI();
  this->initConnections();
}

void PrepareInstallFrame::updateDescription() {
  const QStringList desc_list(delegate_->getOperationDescriptions());
  const QString prefix("•   ");
  QStringList modified_desc_list;
  int max_width = 0;
  QFontMetrics metrics(description_edit_->font());
  for (const QString& desc_item : desc_list) {
    const QString content = prefix + desc_item;
    modified_desc_list.append(content);
    max_width = qMax(metrics.width(content), max_width);
  }
  const QString description = modified_desc_list.join("\n");
  qDebug() << "description:" << description;
  description_edit_->setPlainText(description);
  Q_ASSERT(max_width >= 0);
  // Add horizontal margins.
  description_edit_->setFixedWidth(max_width + 20);
}

void PrepareInstallFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Prepare for Installation"));
    comment_label_->setText(
        tr("Please backup important data and confirm "
           "the following operations"));
    subtitle_label_->setText(
        tr("The following operations will be executed, "
           "please confirm and continue to avoid data loss"));
    abort_button_->setText(tr("Back"));
    continue_button_->setText(tr("Continue"));
  } else {
    QFrame::changeEvent(event);
  }
}

void PrepareInstallFrame::initConnections() {
  connect(abort_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::aborted);
  connect(continue_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::finished);
}

void PrepareInstallFrame::initUI() {
  title_label_ = new TitleLabel(tr("Prepare for Installation"));
  comment_label_ = new CommentLabel(
      tr("Please backup important data and confirm the following operations"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  subtitle_label_ = new QLabel(
      tr("The following operations will be executed, "
         "please confirm and continue to avoid data loss"));
  subtitle_label_->setObjectName("subtitle_label");

  description_edit_ = new QTextEdit();
  description_edit_->setObjectName("description_edit");
  description_edit_->setContentsMargins(10, 10, 10, 10);
  description_edit_->setAcceptRichText(false);
  description_edit_->setReadOnly(true);
  description_edit_->setContextMenuPolicy(Qt::NoContextMenu);
  description_edit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  description_edit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  abort_button_ = new NavButton(tr("Back"));
  continue_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(subtitle_label_, 0, Qt::AlignCenter);
  layout->addWidget(description_edit_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);
  layout->addSpacing(kNavButtonVerticalSpacing);
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/prepare_install_frame.css"));
}

}  // namespace installer