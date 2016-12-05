// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
