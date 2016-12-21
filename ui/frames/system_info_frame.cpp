// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/system_info_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QStackedLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/timezone.h"
#include "ui/frames/inner/system_info_avatar_frame.h"
#include "ui/frames/inner/system_info_form_frame.h"
#include "ui/frames/inner/system_info_keyboard_frame.h"
#include "ui/frames/inner/system_info_timezone_frame.h"
#include "ui/widgets/icon_button.h"

namespace installer {

namespace {

const int kInvalidPageId = -1;
const int kAvatarPageId = 0;
const int kFormPageId = 1;
const int kKeyboardPageId = 2;
const int kTimezonePageId = 3;

}  // namespace

SystemInfoFrame::SystemInfoFrame(QWidget* parent)
    : QFrame(parent),
      last_page_(kInvalidPageId),
      disable_keyboard_(GetSettingsBool(kSystemInfoDisableKeyboardPage)),
      disable_timezone_(GetSettingsBool(kSystemInfoDisableTimezonePage)) {
  this->setObjectName("system_info_frame");

  this->initUI();
  this->initConnections();

  // Read default avatar explicitly.
  avatar_frame_->readConf();

  // Read default keyboard layout.
  keyboard_frame_->readConf();

  this->showFormPage();
}

void SystemInfoFrame::readTimezone() {
  timezone_frame_->readConf();
}

void SystemInfoFrame::writeConf() {
  avatar_frame_->writeConf();
  form_frame_->writeConf();
  keyboard_frame_->writeConf();
  timezone_frame_->writeConf();
}

void SystemInfoFrame::initConnections() {
  connect(avatar_frame_, &SystemInfoAvatarFrame::finished,
          this, &SystemInfoFrame::showFormPage);
  connect(avatar_frame_, &SystemInfoAvatarFrame::avatarUpdated,
          form_frame_, &SystemInfoFormFrame::updateAvatar);

  // Save settings when finished signal is emitted.
  connect(form_frame_, &SystemInfoFormFrame::avatarClicked,
          this, &SystemInfoFrame::showAvatarPage);
  connect(form_frame_, &SystemInfoFormFrame::finished,
          this, &SystemInfoFrame::writeConf);
  connect(form_frame_, &SystemInfoFormFrame::finished,
          this, &SystemInfoFrame::finished);

  connect(keyboard_frame_, &SystemInfoKeyboardFrame::finished,
          this, &SystemInfoFrame::restoreLastPage);
  connect(keyboard_frame_, &SystemInfoKeyboardFrame::layoutUpdated,
          this, &SystemInfoFrame::updateLayout);

  connect(timezone_frame_, &SystemInfoTimezoneFrame::finished,
          this, &SystemInfoFrame::restoreLastPage);
  // Update timezone label when current timezone is updated.
  connect(timezone_frame_, &SystemInfoTimezoneFrame::hideTimezone,
          this, &SystemInfoFrame::hideTimezone);
  connect(timezone_frame_, &SystemInfoTimezoneFrame::timezoneUpdated,
          this, &SystemInfoFrame::updateTimezone);

  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoFrame::showTimezonePage);
  connect(keyboard_button_, &QPushButton::clicked,
          this, &SystemInfoFrame::showKeyboardPage);
}

void SystemInfoFrame::initUI() {
  timezone_button_ = new IconButton(":/images/timezone_normal.svg",
                                    ":/images/timezone_hover.svg",
                                    ":/images/timezone_press.svg",
                                    128, 20, nullptr);
  keyboard_button_ = new IconButton(":/images/keyboard_normal.png",
                                    ":/images/keyboard_hover.png",
                                    ":/images/keyboard_press.png",
                                    128, 20, nullptr);
  head_layout_ = new QHBoxLayout();
  head_layout_->setContentsMargins(20, 0, 0, 0);
  head_layout_->setSpacing(20);
  head_layout_->addWidget(timezone_button_);
  head_layout_->addWidget(keyboard_button_);
  head_layout_->addStretch();

  avatar_frame_ = new SystemInfoAvatarFrame();
  form_frame_ = new SystemInfoFormFrame();
  keyboard_frame_ = new SystemInfoKeyboardFrame();
  timezone_frame_ = new SystemInfoTimezoneFrame();

  stacked_layout_ = new QStackedLayout();
  stacked_layout_->addWidget(avatar_frame_);
  stacked_layout_->addWidget(form_frame_);
  stacked_layout_->addWidget(keyboard_frame_);
  stacked_layout_->addWidget(timezone_frame_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addLayout(head_layout_);
  layout->addLayout(stacked_layout_);

  this->setLayout(layout);
}

void SystemInfoFrame::updateHeadBar() {
  const QString name = stacked_layout_->currentWidget()->objectName();
  const int page = stacked_layout_->currentIndex();

  // Only show header bar in avatar page and form page.
  if (page == kAvatarPageId || page == kFormPageId) {
    keyboard_button_->setVisible(!disable_keyboard_);
    timezone_button_->setVisible(!disable_timezone_);
  } else {
    timezone_button_->hide();
    keyboard_button_->hide();
  }
}

void SystemInfoFrame::restoreLastPage() {
  if (last_page_ != kInvalidPageId) {
    stacked_layout_->setCurrentIndex(last_page_);
  } else {
    stacked_layout_->setCurrentWidget(form_frame_);
  }
  this->updateHeadBar();
}

void SystemInfoFrame::showAvatarPage() {
  if (!GetSettingsBool(kSystemInfoDisableAvatorPage)) {
    stacked_layout_->setCurrentWidget(avatar_frame_);
    this->updateHeadBar();
  }
}

void SystemInfoFrame::showFormPage() {
  stacked_layout_->setCurrentWidget(form_frame_);
  this->updateHeadBar();
}

void SystemInfoFrame::showKeyboardPage() {
  if (!disable_keyboard_) {
    last_page_ = stacked_layout_->currentIndex();
    stacked_layout_->setCurrentWidget(keyboard_frame_);
    this->updateHeadBar();
  }
}

void SystemInfoFrame::showTimezonePage() {
  if (!disable_timezone_) {
    last_page_ = stacked_layout_->currentIndex();
    stacked_layout_->setCurrentWidget(timezone_frame_);
    this->updateHeadBar();
  }
}

void SystemInfoFrame::hideTimezone() {
  disable_timezone_ = true;
  timezone_button_->hide();
}

void SystemInfoFrame::updateLayout(const QString& layout) {
  // Add left margin.
  // TODO(xushaohua): Replace it with css style
  keyboard_button_->setText(QString(" %1").arg(layout));
}

void SystemInfoFrame::updateTimezone(const QString& timezone) {
  // Add left margin.
  timezone_button_->setText(QString(" %1").arg(GetTimezoneName(timezone)));
}

}  // namespace installer