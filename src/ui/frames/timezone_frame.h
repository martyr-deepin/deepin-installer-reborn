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

#ifndef INSTALLER_UI_FRAMES_TIMEZONE_FRAME_H
#define INSTALLER_UI_FRAMES_TIMEZONE_FRAME_H

#include <QFrame>

#include "sysinfo/timezone.h"

namespace installer {

class CommentLabel;
class NavButton;
class TimezoneManager;
class TimezoneMap;
class TitleLabel;

// Displays a world map to let user select timezone.
class TimezoneFrame : public QFrame {
  Q_OBJECT

 public:
  explicit TimezoneFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when back button is clicked.
  void finished();

  // Emit this signal to request to hide timezone page and timezone button.
  void hideTimezone();

  // Emitted when a new timezone is chosen.
  void timezoneUpdated(const QString& timezone);

 public slots:
  // Read default timezone and emit timezoneUpdated() signal.
  void readConf();

  // Update timezone when new language is selected.
  void updateTimezoneBasedOnLanguage(const QString& timezone);

  // Validate timezone and write to conf file.
  void writeConf();

 protected:
  void changeEvent(QEvent* event) override;

  // Remark current timezone when current frame is raised.
  void showEvent(QShowEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Convert timezone alias to its original name.
  QString parseTimezoneAlias(const QString& timezone);

  QString timezone_;
  TimezoneAliasMap alias_map_;

  TimezoneManager* timezone_manager_ = nullptr;

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  TimezoneMap* timezone_map_ = nullptr;
  NavButton* next_button_ = nullptr;

  // Priority of timezone: User > Conf > Scan
  enum class TimezoneSource {
    NotSet,  // Timezone not set.
    User,  // Timezone is setup by user.
    Conf,  // Timezone is read from conf file
    Scan,  // Timezone is updated based on geoip or regdomain
    Language,  // Timezone is setup based on current selected language.
  };
  TimezoneSource timezone_source_;

 private slots:
  void onNextButtonClicked();

  // Update timezone after receiving signals from timezone manager.
  void onTimezoneManagerUpdated(const QString& timezone);

  // Update timezone after a new one has been chosen by user.
  void onTimezoneMapUpdated(const QString& timezone);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_TIMEZONE_FRAME_H
