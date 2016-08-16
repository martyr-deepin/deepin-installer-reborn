// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SelectBootloaderFrame::SelectBootloaderFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("select_bootloader_frame"));

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::initConnections() {

}

void SelectBootloaderFrame::initUI() {
  TitleLabel* title_label =
      new TitleLabel(tr("Select location for boot loader"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("If you do not understand the detailed settings, "
         "please select default settings"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  back_button_ = new NavButton(tr("Back"));
  QHBoxLayout* back_layout = new QHBoxLayout();
  back_layout->addWidget(back_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(back_layout);

  this->setLayout(layout);
}

}  // namespace ui