// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_avatar_frame.h"

#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kAvatarButtonsPerRow = 7;

// Check whether |avatar| is valid.
bool IsValidAvatar(const QString& avatar) {
  if (avatar.isEmpty()) {
    return false;
  }
  return QFile::exists(avatar);
}

}  // namespace

SystemInfoAvatarFrame::SystemInfoAvatarFrame(QWidget* parent)
  : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_avatar_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoAvatarFrame::autoConf() {
  const QString avatar = GetDefaultAvatar();
  if (!IsValidAvatar(avatar)) {
    qWarning() << "autoConf() got invalid avatar: " << avatar;
    return;
  }

  emit this->avatarUpdated(avatar);
  WriteAvatar(avatar);
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
  TitleLabel* title_label = new TitleLabel(tr("User Avatar"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Select an avatar for your account"));
  chosen_avatar_button_ = new AvatarButton(GetDefaultAvatar());

  QGridLayout* avatars_layout = new QGridLayout();
  avatars_layout->setHorizontalSpacing(20);
  avatars_layout->setVerticalSpacing(40);

  const QStringList avatars = GetAvatars();
  int row = 0;
  int col = 0;
  for (const QString& avatar : avatars) {
    AvatarButton* button = new AvatarButton(avatar);
    connect(button, &QPushButton::clicked,
            this, &SystemInfoAvatarFrame::onAvatarButtonClicked);
    avatars_layout->addWidget(button, row, col);
    col += 1;

    // Go to next row.
    if (col >= kAvatarButtonsPerRow) {
      col = 0;
      row += 1;
    }
  }

  QFrame* avatars_wrapper = new QFrame();
  avatars_wrapper->setLayout(avatars_layout);
  QScrollArea* avatars_scroll_area = new QScrollArea();
  avatars_scroll_area->setWidget(avatars_wrapper);
  avatars_scroll_area->setFixedWidth(760);
  avatars_scroll_area->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  avatars_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  avatars_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  avatars_scroll_area->setStyleSheet("background: transparent;");

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addWidget(timezone_button_, 0, Qt::AlignLeft);
  layout->addStretch(2);
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch(1);
  layout->addWidget(chosen_avatar_button_, 0, Qt::AlignCenter);
  layout->addStretch(1);
  layout->addWidget(avatars_scroll_area, 0, Qt::AlignHCenter);
  layout->addStretch(3);

  this->setLayout(layout);
}

void SystemInfoAvatarFrame::onAvatarButtonClicked() {
  AvatarButton* button = qobject_cast<AvatarButton*>(this->sender());
  Q_ASSERT(button);

  if (button) {
    const QString avatar = button->avatar();
    chosen_avatar_button_->updateIcon(avatar);
    WriteAvatar(avatar);

    emit this->avatarUpdated(avatar);
    emit this->finished();
  }
}

}  // namespace installer