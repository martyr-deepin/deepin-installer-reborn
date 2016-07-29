// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_avatar_frame.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

namespace {

const int kAvatarButtonsPerRow = 7;

}

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
  connect(chosen_avatar_button_, &QPushButton::clicked,
          this, &SystemInfoAvatarFrame::onAvatarButtonClicked);
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

  chosen_avatar_button_ = new AvatarButton(service::GetDefaultAvatar());
  QHBoxLayout* chosen_avatar_layout = new QHBoxLayout();
  chosen_avatar_layout->setAlignment(Qt::AlignCenter);
  chosen_avatar_layout->addWidget(chosen_avatar_button_);

  QGridLayout* avatars_layout = new QGridLayout();
  // TODO(xushaohua): Add spaces and scroll area.
  avatars_layout->setHorizontalSpacing(20);
  avatars_layout->setVerticalSpacing(40);
  QHBoxLayout* avatar_wrapper_layout = new QHBoxLayout();
  avatar_wrapper_layout->setAlignment(Qt::AlignCenter);
  avatar_wrapper_layout->addLayout(avatars_layout);

  const QStringList avatars = service::GetAvatars();
  int row = 0;
  int col = 0;
  for (const QString& avatar : avatars) {
    AvatarButton* button = new AvatarButton(avatar);
    connect(button, &QPushButton::clicked,
            this, &SystemInfoAvatarFrame::onAvatarButtonClicked);
    avatars_layout->addWidget(button, row, col);
    col += 1;

    // To next row.
    if (col > kAvatarButtonsPerRow) {
      col = 0;
      row += 1;
    }
  }

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(timezone_layout);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addLayout(chosen_avatar_layout);
  layout->addLayout(avatar_wrapper_layout);
  layout->addStretch();

  this->setLayout(layout);
}

void SystemInfoAvatarFrame::onAvatarButtonClicked() {
  AvatarButton* button = static_cast<AvatarButton*>(this->sender());
  Q_ASSERT(button);

  const QString avatar = button->avatar();
  chosen_avatar_button_->updateIcon(avatar);
  emit this->avatarUpdated(avatar);
  emit this->finished();
}

}  // namespace ui