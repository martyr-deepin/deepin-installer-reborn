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
#include "ui/frames/consts.h"
#include "ui/widgets/icon_button.h"

namespace ui {

SystemInfoFrame::SystemInfoFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_frame"));

  this->initUI();
  this->initConnections();

  this->showFormPage();
}

void SystemInfoFrame::initConnections() {
  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoFrame::showTimezonePage);

  connect(avatar_frame_, &SystemInfoAvatarFrame::finished,
          this, &SystemInfoFrame::showFormPage);
  connect(form_frame_, &SystemInfoFormFrame::finished,
          this, &SystemInfoFrame::finished);
  connect(form_frame_, &SystemInfoFormFrame::avatarClicked,
          this, &SystemInfoFrame::showAvatarPage);
  connect(timezone_frame_, &SystemInfoTimezoneFrame::finished,
          this, &SystemInfoFrame::showFormPage);
}

void SystemInfoFrame::initUI() {
  avatar_frame_ = new SystemInfoAvatarFrame();
  form_frame_ = new SystemInfoFormFrame();
  timezone_frame_ = new SystemInfoTimezoneFrame();

  stacked_layout_ = new QStackedLayout();
  stacked_layout_->addWidget(avatar_frame_);
  stacked_layout_->addWidget(form_frame_);
  stacked_layout_->addWidget(timezone_frame_);

  timezone_button_ = new IconButton(QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    128, 32, nullptr);
  // TODO(xushaohua): Remove timezone text.
  timezone_button_->setText("Beijing");
  QHBoxLayout* timezone_layout = new QHBoxLayout();
  timezone_layout->setAlignment(Qt::AlignLeft);
  timezone_layout->addWidget(timezone_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(timezone_layout);
  layout->addLayout(stacked_layout_);

  this->setLayout(layout);
}

void SystemInfoFrame::showAvatarPage() {
  if (service::GetSettingsBool(service::kSystemInfoDisableAvatorPage)) {
    return;
  }
  timezone_button_->setVisible(true);
  stacked_layout_->setCurrentWidget(avatar_frame_);
}

void SystemInfoFrame::showFormPage() {
  timezone_button_->setVisible(true);
  stacked_layout_->setCurrentWidget(form_frame_);
}

void SystemInfoFrame::showTimezonePage() {
  if (service::GetSettingsBool(service::kSystemInfoDisableTimezonePage)) {
    return;
  }
  timezone_button_->setVisible(false);
  stacked_layout_->setCurrentWidget(timezone_frame_);
}

}  // namespace ui