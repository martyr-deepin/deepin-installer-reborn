// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H

#include <QFrame>

namespace ui {

class NavButton;

// Displays a world map to let user select timezone.
class SystemInfoTimezoneFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoTimezoneFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when back button is clicked.
  void finished();

  // Emitted when a new timezone is chosen.
  void timezoneUpdated(const QString& timezone);

 private:
  void initConnections();
  void initUI();

  NavButton* back_button_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H
