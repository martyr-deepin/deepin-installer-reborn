// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_timezone_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/timezone.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SystemInfoTimezoneFrame::SystemInfoTimezoneFrame(QWidget* parent)
    : QFrame(parent),
      timezone_() {
  this->setObjectName(QStringLiteral("system_info_timezone_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoTimezoneFrame::autoConf() {
  QString timezone;
  if (service::GetSettingsBool(service::kSystemInfoUseDefaultTimezoneName)) {
    timezone = service::GetSettingsString(
        service::kSystemInfoDefaultTimezoneName);
  }

  if (!sysinfo::IsValidTimezone(timezone)) {
    timezone = sysinfo::GetCurrentTimezone();
  }

  if (!sysinfo::IsValidTimezone(timezone)) {
    qWarning() << "autoConf() got invalid timezone:" << timezone;
    return;
  }

  timezone_ = timezone;
  emit this->timezoneUpdated(sysinfo::GetTimezoneName(timezone));
  service::WriteTimezone(timezone);
}

void SystemInfoTimezoneFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SystemInfoTimezoneFrame::finished);
}

void SystemInfoTimezoneFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Select Time Zone"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label =
      new CommentLabel(tr("Mark your zone in the map"));
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