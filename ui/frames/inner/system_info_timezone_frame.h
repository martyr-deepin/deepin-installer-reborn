// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H

#include <QFrame>

namespace installer {

class NavButton;

// Displays a world map to let user select timezone.
class SystemInfoTimezoneFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoTimezoneFrame(QWidget* parent = nullptr);

  void autoConf();

 signals:
  // Emitted when back button is clicked.
  void finished();

  // Emitted when a new timezone is chosen.
  void timezoneUpdated(const QString& timezone);

 private:
  void initConnections();
  void initUI();

  QString timezone_;

  NavButton* back_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_TIMEZONE_FRAME_H
