// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/prepare_install_frame.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include "base/file_util.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Prepare for Installation";
const char kTextComment[] =
    "Please backup important data and confirm the following operations";
const char kTextSubtitle[] = "The following operations will be executed, "
                             "please confirm and continue to avoid data loss";
const char kTextBack[] = "Back";
const char kTextContinue[] = "Continue";

}  // namespace

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
  for (const QString& desc_item : desc_list) {
    modified_desc_list.append(prefix + desc_item);
  }
  // NOTE(xushaohua): Add tow line breaks to simulate line-height property
  // in QLabel.
  const QString description = modified_desc_list.join("\n\n");
  qDebug() << "description:" << description;
  description_label_->setText(description);
}

void PrepareInstallFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    subtitle_label_->setText(tr(kTextSubtitle));
    abort_button_->setText(tr(kTextBack));
    continue_button_->setText(tr(kTextContinue));
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
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  subtitle_label_ = new QLabel(tr(kTextSubtitle));
  subtitle_label_->setObjectName("subtitle_label");

  description_label_ = new QLabel();
  description_label_->setObjectName("description_label");
  QScrollArea* description_scroll_area = new QScrollArea();
  description_scroll_area->setObjectName("description_area");
  description_scroll_area->setContentsMargins(0, 0, 0, 0);
  description_scroll_area->setWidget(description_label_);
  description_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  description_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  description_scroll_area->setWidgetResizable(true);
  QSizePolicy description_size_policy(QSizePolicy::Maximum,
                                      QSizePolicy::Preferred);
  description_size_policy.setHorizontalStretch(1);
  description_size_policy.setVerticalStretch(10);
  description_scroll_area->setSizePolicy(description_size_policy);
  QHBoxLayout* description_wrapper_layout = new QHBoxLayout();
  description_wrapper_layout->setContentsMargins(0, 0, 0, 0);
  description_wrapper_layout->setSpacing(0);
  description_wrapper_layout->addStretch(2);
  description_wrapper_layout->addWidget(description_scroll_area);
  description_wrapper_layout->addStretch(2);

  abort_button_ = new NavButton(tr(kTextBack));
  continue_button_ = new NavButton(tr(kTextContinue));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch(1);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch(1);
  layout->addWidget(subtitle_label_, 0, Qt::AlignCenter);
  layout->addLayout(description_wrapper_layout);
  layout->addStretch(1);
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/prepare_install_frame.css"));
}

}  // namespace installer