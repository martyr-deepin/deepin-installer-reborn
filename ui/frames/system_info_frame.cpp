// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/system_info_frame.h"

#include <QStackedLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/inner/system_info_avatar_frame.h"
#include "ui/frames/inner/system_info_form_frame.h"
#include "ui/frames/inner/system_info_timezone_frame.h"

namespace installer {

SystemInfoFrame::SystemInfoFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("system_info_frame");

  this->initUI();
  this->initConnections();

  // Read default avatar and timezone explicitly.
  avatar_frame_->readConf();
  timezone_frame_->readConf();

  this->showFormPage();
}

void SystemInfoFrame::writeConf() {
  avatar_frame_->writeConf();
  form_frame_->writeConf();
  timezone_frame_->writeConf();
}

void SystemInfoFrame::initConnections() {
  connect(avatar_frame_, &SystemInfoAvatarFrame::finished,
          this, &SystemInfoFrame::showFormPage);
  connect(avatar_frame_, &SystemInfoAvatarFrame::timezoneClicked,
          this, &SystemInfoFrame::showTimezonePage);
  connect(avatar_frame_, &SystemInfoAvatarFrame::avatarUpdated,
          form_frame_, &SystemInfoFormFrame::updateAvatar);
  connect(form_frame_, &SystemInfoFormFrame::avatarClicked,
          this, &SystemInfoFrame::showAvatarPage);
  connect(form_frame_, &SystemInfoFormFrame::timezoneClicked,
          this, &SystemInfoFrame::showTimezonePage);
  connect(timezone_frame_, &SystemInfoTimezoneFrame::finished,
          this, &SystemInfoFrame::showFormPage);

  // Save settings when finished signal is emitted.
  connect(form_frame_, &SystemInfoFormFrame::finished,
          this, &SystemInfoFrame::writeConf);
  connect(form_frame_, &SystemInfoFormFrame::finished,
          this, &SystemInfoFrame::finished);

  // Update timezone label when current timezone is updated.
  connect(timezone_frame_, &SystemInfoTimezoneFrame::timezoneUpdated,
          avatar_frame_, &SystemInfoAvatarFrame::updateTimezone);
  connect(timezone_frame_, &SystemInfoTimezoneFrame::timezoneUpdated,
          form_frame_, &SystemInfoFormFrame::updateTimezone);
}

void SystemInfoFrame::initUI() {
  avatar_frame_ = new SystemInfoAvatarFrame();
  form_frame_ = new SystemInfoFormFrame();
  timezone_frame_ = new SystemInfoTimezoneFrame();

  stacked_layout_ = new QStackedLayout();
  stacked_layout_->addWidget(avatar_frame_);
  stacked_layout_->addWidget(form_frame_);
  stacked_layout_->addWidget(timezone_frame_);

  this->setLayout(stacked_layout_);
}

void SystemInfoFrame::showAvatarPage() {
  if (!GetSettingsBool(kSystemInfoDisableAvatorPage)) {
    stacked_layout_->setCurrentWidget(avatar_frame_);
    avatar_frame_->raise();
  }
}

void SystemInfoFrame::showFormPage() {
  stacked_layout_->setCurrentWidget(form_frame_);
  form_frame_->raise();
}

void SystemInfoFrame::showTimezonePage() {
  if (!GetSettingsBool(kSystemInfoDisableTimezonePage)) {
    stacked_layout_->setCurrentWidget(timezone_frame_);
    timezone_frame_->raise();
  }
}

}  // namespace installer