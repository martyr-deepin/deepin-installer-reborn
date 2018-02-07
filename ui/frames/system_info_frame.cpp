/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui/frames/system_info_frame.h"

#include <QHBoxLayout>
#include <QStackedLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/inner/system_info_avatar_frame.h"
#include "ui/frames/inner/system_info_form_frame.h"
#include "ui/frames/inner/system_info_keyboard_frame.h"
#include "timezone_frame.h"
#include "ui/widgets/pointer_button.h"

namespace installer {

namespace {

const int kInvalidPageId = -1;
const int kAvatarPageId = 0;
const int kFormPageId = 1;

}  // namespace

SystemInfoFrame::SystemInfoFrame(QWidget* parent)
    : QFrame(parent),
      last_page_(kInvalidPageId),
      disable_keyboard_(GetSettingsBool(kSystemInfoDisableKeyboardPage)) {
  this->setObjectName("system_info_frame");

  this->initUI();
  this->initConnections();

  this->showFormPage();
}

void SystemInfoFrame::readConf() {
  // Read default avatar explicitly.
  avatar_frame_->readConf();

  // Read default keyboard layout.
  keyboard_frame_->readConf();
}

void SystemInfoFrame::writeConf() {
  // Notify sub-pages to save settings.
  avatar_frame_->writeConf();
  form_frame_->writeConf();
  keyboard_frame_->writeConf();
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

  connect(keyboard_button_, &QPushButton::clicked,
          this, &SystemInfoFrame::showKeyboardPage);
}

void SystemInfoFrame::initUI() {
  keyboard_button_ = new PointerButton();
  keyboard_button_->setObjectName("keyboard_button");
  keyboard_button_->setFlat(true);
  keyboard_button_->setFixedHeight(23);

  head_layout_ = new QHBoxLayout();
  head_layout_->setContentsMargins(30, 0, 0, 0);
  head_layout_->setSpacing(30);
  head_layout_->addWidget(keyboard_button_);
  head_layout_->addStretch();

  avatar_frame_ = new SystemInfoAvatarFrame();
  form_frame_ = new SystemInfoFormFrame();
  keyboard_frame_ = new SystemInfoKeyboardFrame();

  stacked_layout_ = new QStackedLayout();
  stacked_layout_->setContentsMargins(0, 0, 0, 0);
  stacked_layout_->setSpacing(0);
  stacked_layout_->addWidget(avatar_frame_);
  stacked_layout_->addWidget(form_frame_);
  stacked_layout_->addWidget(keyboard_frame_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addLayout(head_layout_);
  layout->addLayout(stacked_layout_);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/system_info_frame.css"));
}

void SystemInfoFrame::updateHeadBar() {
  const QString name = stacked_layout_->currentWidget()->objectName();
  const int page = stacked_layout_->currentIndex();

  // Only show header bar in avatar page and form page.
  if (page == kAvatarPageId || page == kFormPageId) {
    keyboard_button_->setVisible(!disable_keyboard_);
  } else {
    keyboard_button_->hide();
  }
}

void SystemInfoFrame::restoreLastPage() {
  if (last_page_ != kInvalidPageId) {
    stacked_layout_->setCurrentIndex(last_page_);
  } else {
    // Displays default page if last_page_ is not set.
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

void SystemInfoFrame::updateLayout(const QString& layout) {
  keyboard_button_->setText(layout);
}

}  // namespace installer
