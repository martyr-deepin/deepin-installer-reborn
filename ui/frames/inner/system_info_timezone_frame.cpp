// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_timezone_frame.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

#include "partman/os_prober.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "service/timezone_manager.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/timezone_map.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

// Set timezone to UTC when local-time is used.
const char kDefaultTimezone[] = "Etc/UTC";

// Check if any Windows partition is found on disk.
bool HasWindowsPartition() {
  const OsProberItems os_items = GetOsProberItems();
  for (const OsProberItem& item : os_items) {
    if (item.type == OsType::Windows) {
      return true;
    }
  }
  return false;
}

}  // namespace

SystemInfoTimezoneFrame::SystemInfoTimezoneFrame(QWidget* parent)
    : QFrame(parent),
      timezone_(),
      alias_map_(GetTimezoneAliasMap()),
      timezone_manager_(new TimezoneManager(this)),
      timezone_source_(TimezoneSource::NotSet) {
  this->setObjectName("system_info_timezone_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoTimezoneFrame::readConf() {
  // Policy:
  //  * Call `os-prober` if both "partition_enable_os_prober" and
  //    "system_info_use_windows_time" are enabled.
  //  * Hide timezone-page and timezone-button if
  //    "system_info_windows_disable_timezone_page" if enabled.
  //  * If no windows partition found, then:
  //    * Read default timezone from settings.
  //    * Scan wifi spot.
  //    * Send http request to get geo ip.
  //    * Or wait for user to choose timezone on map.

  if (GetSettingsBool(kPartitionEnableOsProber) &&
      GetSettingsBool(kSystemInfoUseWindowsTime) &&
      HasWindowsPartition()) {
    // If local time is used, set timezone to Etc/UTC.
    timezone_ = kDefaultTimezone;
    timezone_source_ = TimezoneSource::Local;
    if (GetSettingsBool(kSystemInfoWindowsDisableTimezonePage)) {
      // Send hide-timezone signal.
      emit this->hideTimezone();
      // Do not send timezoneUpdated() signal.
      return;
    }
  } else {
    // Read timezone from settings.
    timezone_ = GetSettingsString(kSystemInfoDefaultTimezone);
    timezone_ = this->parseTimezoneAlias(timezone_);
    if (IsValidTimezone(timezone_)) {
      timezone_source_ = TimezoneSource::Conf;
    } else {
      const bool use_geoip = GetSettingsBool(kSystemInfoTimezoneUseGeoIp);
      const bool use_regdomain =
          GetSettingsBool(kSystemInfoTimezoneUseRegdomain);
      timezone_manager_->update(use_geoip, use_regdomain);

      // Use default timezone.
      timezone_ = kDefaultTimezone;
    }
  }
  emit this->timezoneUpdated(timezone_);
}

void SystemInfoTimezoneFrame::timezoneUpdatedByLanguage(
    const QString& timezone) {
  // Check priority.
  if (timezone_source_ == TimezoneSource::Local ||
      timezone_source_ == TimezoneSource::User ||
      timezone_source_ == TimezoneSource::Conf) {
    qDebug() << "Ignores language default timezone:" << timezone;
    return;
  }

  if (IsValidTimezone(timezone)) {
    timezone_source_ = TimezoneSource::Language;
    timezone_ = timezone;
    emit this->timezoneUpdated(timezone_);
  }
}

void SystemInfoTimezoneFrame::writeConf() {
  // Validate timezone before writing to settings file.
  if (IsValidTimezone(timezone_)) {
    const bool is_local_time = (timezone_source_ == TimezoneSource::Local);
    WriteTimezone(timezone_, is_local_time);
  } else {
    qWarning() << "Invalid timezone:" << timezone_;
  }
}

void SystemInfoTimezoneFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Select Time Zone"));
    comment_label_->setText(tr("Mark your zone on the map"));
    back_button_->setText(tr("Back"));

    // Also update timezone.
    if (!timezone_.isEmpty()) {
      timezone_map_->setTimezone(timezone_);
      emit this->timezoneUpdated(timezone_);
    }
  } else {
    QFrame::changeEvent(event);
  }
}

void SystemInfoTimezoneFrame::showEvent(QShowEvent* event) {
  QFrame::showEvent(event);

  // NOTE(xushaohua): Add a delay to wait for paint event of timezone map.
  QTimer::singleShot(0, [&]() {
    timezone_map_->setTimezone(timezone_map_->getTimezone());
  });
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
  title_label_ = new TitleLabel(tr("Select Time Zone"));
  comment_label_ = new CommentLabel(tr("Mark your zone on the map"));
  timezone_map_ = new TimezoneMap(this);
  back_button_ = new NavButton(tr("Back"));

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
  this->setContentsMargins(0, 0, 0, 0);
}

QString SystemInfoTimezoneFrame::parseTimezoneAlias(const QString& timezone) {
  // If |timezone| not in alias map, returns itself.
  return alias_map_.value(timezone, timezone);
}

void SystemInfoTimezoneFrame::onBackButtonClicked() {
  if (IsValidTimezone(timezone_)) {
    emit this->timezoneUpdated(timezone_);
  } else {
    qWarning() << "Invalid timezone:" << timezone_;
  }
  emit this->finished();
}

void SystemInfoTimezoneFrame::onTimezoneManagerUpdated(
    const QString& timezone) {
  // Check priority.
  if (timezone_source_ == TimezoneSource::NotSet ||
      timezone_source_ == TimezoneSource::Scan) {
    // Update timezone only if it is not set.
    timezone_source_ = TimezoneSource::Scan;
    timezone_ = this->parseTimezoneAlias(timezone);
    emit this->timezoneUpdated(timezone_);
  } else {
    qDebug() << "Ignore timezone value from timezone-manager:" << timezone;
  }
}

void SystemInfoTimezoneFrame::onTimezoneMapUpdated(const QString& timezone) {
  timezone_source_ = TimezoneSource::User;
  // No need to convert timezone alias.
  timezone_ = timezone;
  emit this->timezoneUpdated(timezone_);
}

}  // namespace installer