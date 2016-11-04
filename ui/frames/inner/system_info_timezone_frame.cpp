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

namespace installer {

SystemInfoTimezoneFrame::SystemInfoTimezoneFrame(QWidget* parent)
    : QFrame(parent),
      timezone_() {
  this->setObjectName("system_info_timezone_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoTimezoneFrame::autoConf() {
  QString timezone;
  if (GetSettingsBool(kSystemInfoUseDefaultTimezone)) {
    timezone = GetSettingsString(kSystemInfoDefaultTimezone);
  }

  if (!IsValidTimezone(timezone)) {
    timezone = GetCurrentTimezone();
  }

  if (!IsValidTimezone(timezone)) {
    qWarning() << "autoConf() got invalid timezone:" << timezone;
    return;
  }

  timezone_ = timezone;
  emit this->timezoneUpdated(GetTimezoneName(timezone));
  WriteTimezone(timezone);
}

void SystemInfoTimezoneFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SystemInfoTimezoneFrame::finished);
}

void SystemInfoTimezoneFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Select Time Zone"));
  CommentLabel* comment_label =
      new CommentLabel(tr("Mark your zone in the map"));
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