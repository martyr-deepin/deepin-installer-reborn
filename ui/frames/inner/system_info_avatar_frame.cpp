// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_avatar_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SystemInfoAvatarFrame::SystemInfoAvatarFrame(QWidget* parent)
  : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_avatar_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoAvatarFrame::updateTimezone(const QString& timezone) {
  timezone_button_->setText(timezone);
}

void SystemInfoAvatarFrame::initConnections() {
  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoAvatarFrame::timezoneClicked);
}

void SystemInfoAvatarFrame::initUI() {
  timezone_button_ = new IconButton(QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    128, 32, nullptr);
  // TODO(xushaohua): Remove timezone text.
  timezone_button_->setText("Beijing");
  QHBoxLayout* timezone_layout = new QHBoxLayout();
  timezone_layout->setAlignment(Qt::AlignLeft);
  timezone_layout->addWidget(timezone_button_);

  TitleLabel* title_label = new TitleLabel(tr("User Avatar"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Select an avatar for your account"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(timezone_layout);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();

  this->setLayout(layout);
}

}  // namespace ui