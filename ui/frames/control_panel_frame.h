// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H
#define INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H

#include <QFrame>
class QTextEdit;
class QTimer;

namespace installer {

// Displays log file content and settings info in debug mode.
class ControlPanelFrame : public QFrame {
  Q_OBJECT

 public:
  explicit ControlPanelFrame(QWidget* parent = nullptr);

 public slots:
  void toggleVisible();

 private:
  void initConnections();
  void initUI();

  QTextEdit* log_viewer_ = nullptr;
  QTimer* timer_ = nullptr;
  QString log_file_path_;
  QString log_content_;

 private slots:
  void onTimerTimeout();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H
