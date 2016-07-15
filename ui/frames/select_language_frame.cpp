// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/select_language_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"

namespace ui {

SelectLanguageFrame::SelectLanguageFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("select_language_frame"));

  this->initUI();
}

void SelectLanguageFrame::initUI() {
  // TODO(xushaohua): Add deepin logo.

  CommentLabel* comment_label =
      new CommentLabel("Please select the language you want to use\n"
                       "请选择您的语言");
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  // TODO(xushaohua): Add language list.

  NavButton* next_button = new NavButton(tr("Next"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(next_layout);

  this->setLayout(layout);

  connect(next_button, &QPushButton::clicked,
          this, &SelectLanguageFrame::onNextButtonClicked);
}

void SelectLanguageFrame::onNextButtonClicked() {
  // TODO(xushaohua): For test only.
//  if (language_is_selected_) {
  if (true) {
    emit this->finished();
  }
}

}  // namespace ui