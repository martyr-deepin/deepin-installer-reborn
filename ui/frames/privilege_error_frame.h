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

#ifndef INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H
#define INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H

#include <QFrame>

namespace installer {

class NavButton;

// Display this page when root privilege is not guaranteed.
class PrivilegeErrorFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PrivilegeErrorFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when continue_button_ is clicked.
  void finished();

 private:
  void initConnection();
  void initUI();

  NavButton* continue_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H
