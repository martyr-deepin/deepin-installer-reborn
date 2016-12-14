// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_timezone_frame.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "service/timezone_manager.h"
#include "sysinfo/timezone.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/timezone_map.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Select Time Zone";
const char kTextComment[] = "Mark your zone in the map";
const char kTextBack[] = "Back";

}  // namespace

SystemInfoTimezoneFrame::SystemInfoTimezoneFrame(QWidget* parent)
    : QFrame(parent),
      timezone_(),
      timezone_manager_(new TimezoneManager(this)),
      timezone_source_(TimezoneSource::NotSet) {
  this->setObjectName("system_info_timezone_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoTimezoneFrame::readConf() {
  timezone_ = GetSettingsString(kSystemInfoDefaultTimezone);
  if (IsValidTimezone(timezone_)) {
    timezone_source_ = TimezoneSource::Conf;
    emit this->timezoneUpdated(GetTimezoneName(timezone_));
  } else {
    const bool use_geoip = GetSettingsBool(kSystemInfoTimezoneUseGeoIp);
    const bool use_regdomain = GetSettingsBool(kSystemInfoTimezoneUseRegdomain);
    timezone_manager_->update(use_geoip, use_regdomain);
  }
}

void SystemInfoTimezoneFrame::writeConf() {
  if (IsValidTimezone(timezone_)) {
    WriteTimezone(timezone_);
  } else {
    qWarning() << "Invalid timezone:" << timezone_;
  }
}

void SystemInfoTimezoneFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    back_button_->setText(tr(kTextBack));
  } else {
    QFrame::changeEvent(event);
  }
}

void SystemInfoTimezoneFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &SystemInfoTimezoneFrame::onBackButtonClicked);

  connect(timezone_manager_, &TimezoneManager::timezoneUpdated,
          this, &SystemInfoTimezoneFrame::onTimezoneManagerUpdated);
  connect(timezone_map_, &TimezoneMap::timezoneUpdated,
          this, &SystemInfoTimezoneFrame::onTimezoneMapUpdated);

  // Remark timezone on map when it is updated.
  connect(this, &SystemInfoTimezoneFrame::timezoneUpdated,
          timezone_map_, &TimezoneMap::setTimezone);
}

void SystemInfoTimezoneFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  timezone_map_ = new TimezoneMap();
  back_button_ = new NavButton(tr(kTextBack));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addWidget(comment_label_, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(timezone_map_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(back_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

void SystemInfoTimezoneFrame::onBackButtonClicked() {
  if (IsValidTimezone(timezone_)) {
    emit this->timezoneUpdated(GetTimezoneName(timezone_));
  } else {
    qWarning() << "Invalid timezone:" << timezone_;
  }
  emit this->finished();
}

void SystemInfoTimezoneFrame::onTimezoneManagerUpdated(
    const QString& timezone) {
  if (timezone_source_ == TimezoneSource::NotSet ||
      timezone_source_ == TimezoneSource::Scan) {
    // Update timezone only if it is not set.
    timezone_source_ = TimezoneSource::Scan;
    timezone_ = timezone;
    emit this->timezoneUpdated(timezone_);
  } else {
    qDebug() << "Ignore timezone value from timezone-manager:" << timezone;
  }
}

void SystemInfoTimezoneFrame::onTimezoneMapUpdated(const QString& timezone) {
  timezone_source_ = TimezoneSource::User;
  timezone_ = timezone;
  emit this->timezoneUpdated(timezone_);
}

}  // namespace installer