// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_avatar_frame.h"

#include <QDebug>
#include <QEvent>
#include <QFile>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/avatar_list_delegate.h"
#include "ui/frames/consts.h"
#include "ui/views/pointer_list_view.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "User Avatar";
const char kTextComment[] = "Select an avatar for your account";

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
  this->setObjectName("system_info_avatar_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoAvatarFrame::hideTimezoneButton() {
  timezone_button_->setVisible(false);
}

void SystemInfoAvatarFrame::readConf() {
  const QString avatar = current_avatar_button_->avatar();
  emit this->avatarUpdated(avatar);
}

void SystemInfoAvatarFrame::writeConf() {
  const QString avatar = current_avatar_button_->avatar();
  if (IsValidAvatar(avatar)) {
    WriteAvatar(avatar);
  } else {
    qWarning() << "Invalid avatar: " << avatar;
  }
}

void SystemInfoAvatarFrame::updateTimezone(const QString& timezone) {
  // Add left margin.
  timezone_button_->setText(QString(" %1").arg(timezone));
}

void SystemInfoAvatarFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
  } else {
    QFrame::changeEvent(event);
  }
}

void SystemInfoAvatarFrame::initConnections() {
  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoAvatarFrame::timezoneClicked);

  // Return to previous page when chosen_avatar_button is clicked.
  connect(current_avatar_button_, &QPushButton::clicked,
          this, &SystemInfoAvatarFrame::finished);
  connect(list_view_, &QListView::pressed,
          this, &SystemInfoAvatarFrame::onListViewPressed);
}

void SystemInfoAvatarFrame::initUI() {
  timezone_button_ = new IconButton(":/images/timezone_normal.svg",
                                    ":/images/timezone_hover.svg",
                                    ":/images/timezone_press.svg",
                                    128, 20, nullptr);
  QHBoxLayout* timezone_layout = new QHBoxLayout();
  timezone_layout->setContentsMargins(0, 0, 0, 0);
  timezone_layout->setSpacing(0);
  timezone_layout->addSpacing(20);
  timezone_layout->addWidget(timezone_button_);
  timezone_layout->addStretch();

  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  // Set default avatar.
  current_avatar_button_ = new AvatarButton(GetDefaultAvatar());

  const QStringList avatars = GetAvatars();
  list_view_ = new PointerListView();
  QStringListModel* list_model = new QStringListModel(avatars, list_view_);
  list_view_->setModel(list_model);
  AvatarListDelegate* list_delegate = new AvatarListDelegate(list_view_);
  list_view_->setItemDelegate(list_delegate);
  QSizePolicy list_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  list_policy.setVerticalStretch(100);
  list_view_->setSizePolicy(list_policy);
  list_view_->setContentsMargins(0, 0, 0, 0);
  list_view_->setSpacing(40);
  list_view_->setAcceptDrops(false);
  list_view_->setWrapping(true);
  list_view_->setUniformItemSizes(true);
  list_view_->setFlow(QListView::LeftToRight);
  list_view_->setViewMode(QListView::IconMode);
  list_view_->setFixedWidth(882);
  list_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  list_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  list_view_->setStyleSheet(ReadFile(":/styles/avatar_list_view.css"));;

  QHBoxLayout* list_layout = new QHBoxLayout();
  list_layout->setContentsMargins(0, 0, 0, 0);
  list_layout->setSpacing(0);
  list_layout->addStretch();
  list_layout->addWidget(list_view_);
  list_layout->addStretch();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(timezone_layout);
  layout->addSpacing(10);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addSpacing(40);
  layout->addWidget(current_avatar_button_, 0, Qt::AlignCenter);
  layout->addSpacing(40);
  layout->addLayout(list_layout);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

void SystemInfoAvatarFrame::onListViewPressed(const QModelIndex& index) {
  const QString avatar = index.model()->data(index).toString();
  if (IsValidAvatar(avatar)) {
    current_avatar_button_->updateIcon(avatar);
    emit this->avatarUpdated(avatar);
  } else {
    qWarning() << "Invalid avatar:" << avatar;
  }
  emit this->finished();
}

}  // namespace installer