// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_LOG_VIEWER_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_LOG_VIEWER_FRAME_H

#include <QFrame>
#include <QTimer>
class QTextEdit;

namespace ui {

// Monitors changes and read content of log file.
class LogViewerFrame : public QFrame {
  Q_OBJECT

 public:
  explicit LogViewerFrame(QWidget* parent = nullptr);

 public slots:
  // Toggle window visibility.
  void toggleVisible();

 protected:
  // Enable timer when window is shown, or else disable it.
  void hideEvent(QHideEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  void initConnection();
  void initUI();

  QTimer timer_;
  QTextEdit* text_edit_ = nullptr;
  QString log_file_path_;
  QString log_content_;

 private slots:
  void onTimerTimeout();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_LOG_VIEWER_FRAME_H
