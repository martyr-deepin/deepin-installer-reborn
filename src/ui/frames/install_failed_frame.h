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

#ifndef INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H

#include <QFrame>
class QLabel;
class QPushButton;

namespace installer {

class CommentLabel;
class NavButton;
class QRWidget;
class TitleLabel;

// Displays error message and a QR image
class InstallFailedFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallFailedFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when reboot button is clocked.
  void finished();

 public slots:
  // Reload error message.
  void updateMessage();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* reboot_button_ = nullptr;
  QRWidget* qr_widget_ = nullptr;
  QLabel* content_label_ = nullptr;
  QPushButton* control_button_ = nullptr;

 private slots:
  void onControlButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_FAILED_FRAME_H
