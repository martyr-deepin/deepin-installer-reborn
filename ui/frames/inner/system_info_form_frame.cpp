// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_form_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SystemInfoFormFrame::SystemInfoFormFrame(QWidget* parent)
  : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_form_frame"));

  this->initUI();
  this->initConnections();
}


void SystemInfoFormFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::onNextButtonClicked);
}

void SystemInfoFormFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Create User Account"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label =
      new CommentLabel(tr("Input username and password"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  next_button_ = new NavButton(tr("Next"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(next_layout);

  this->setLayout(layout);
}

void SystemInfoFormFrame::onNextButtonClicked() {
  if (true) {
    emit this->finished();
  }
}

}  // namespace ui