// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

SelectBootloaderFrame::SelectBootloaderFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("select_bootloader_frame"));

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SelectBootloaderFrame::finished);
}

void SelectBootloaderFrame::initUI() {
  TitleLabel* title_label =
      new TitleLabel(tr("Select location for boot loader"));
  CommentLabel* comment_label = new CommentLabel(
      tr("If you do not understand the detailed settings, "
         "please select default settings"));
  back_button_ = new NavButton(tr("Back"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addWidget(comment_label, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(back_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer