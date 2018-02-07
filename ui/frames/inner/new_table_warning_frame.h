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

#ifndef INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H

#include <QFrame>
class QLabel;

namespace installer {

class CommentLabel;
class NavButton;
class TitleLabel;

// Display warning message before creating new partition table.
class NewTableWarningFrame : public QFrame {
  Q_OBJECT

 public:
  explicit NewTableWarningFrame(QWidget* parent = nullptr);

  // Set/get device_path_.
  QString devicePath() const;
  void setDevicePath(const QString& device_path, const QString& device_info);

 signals:
  // Emitted when cancel_button_ is clicked.
  void canceled();

  // Emitted when confirm_button_ is clicked.
  // |device_path| is the device disk path to be formatted.
  void confirmed(const QString& device_path);

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  QLabel* disk_name_label_ = nullptr;
  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* cancel_button_ = nullptr;
  NavButton* confirm_button_ = nullptr;

  QString device_path_;

 private slots:
  void onConfirmButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_NEW_TABLE_WARNING_FRAME_H
