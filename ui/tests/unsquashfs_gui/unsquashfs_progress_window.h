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

#ifndef INSTALLER_MISC_UNSQUASHFS_PROGRESS_WINDOW_H
#define INSTALLER_MISC_UNSQUASHFS_PROGRESS_WINDOW_H

#include <QFrame>
class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

class UnsquashfsProgressWindow : public QFrame {
  Q_OBJECT

 public:
  explicit UnsquashfsProgressWindow(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  void startWork(const QString& filepath);
  void cancelWork();

  QLabel* filepath_label_ = nullptr;
  QPushButton* ctrl_button_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;
  bool work_running_ = false;
  QTimer* timer_ = nullptr;

 private slots:
  void onCtrlButtonClicked();
  void onReadProgressTimeout();
};

#endif  // INSTALLER_MISC_UNSQUASHFS_PROGRESS_WINDOW_H
