// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H
#define INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H

#include <QFrame>
class QPushButton;
class QStackedWidget;
class QTabBar;
class QTextEdit;
class QTimer;

namespace installer {

class TableComboBox;

// Displays log file content and settings info in debug mode.
class ControlPanelFrame : public QFrame {
  Q_OBJECT

 public:
  explicit ControlPanelFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when page index is changed in page_combo_box_.
  void currentPageChanged(int index);

  // Emitted when refresh_device_button_ is clicked.
  void requestRefreshDevices();

  // Emitted when refresh_qr_button_ is clicked.
  void requestRefreshQR();

  // Emitted when simulate_slide_button_ is clicked.
  void requestSimulateSlide();

 public slots:
  void toggleVisible();

 private:
  void initConnections();
  void initUI();

  QStackedWidget* stacked_widget_ = nullptr;
  QTabBar* tab_bar_ = nullptr;

  // Displays console log
  QTextEdit* log_viewer_ = nullptr;

  // Displays page index
  TableComboBox* page_combo_box_ = nullptr;
  QPushButton* refresh_devices_button_ = nullptr;
  QPushButton* refresh_qr_button_ = nullptr;
  QPushButton* simulate_slide_button_ = nullptr;

  QTextEdit* settings_viewer_ = nullptr;

  QTimer* timer_ = nullptr;
  QString log_file_path_;
  QString log_content_;

 private slots:
  // Highlight current line when position of text cursor is changed.
  void onLogViewerCursorPositionChanged();

  // Switch pages on tab bar changed to |index|.
  void onTabBarChanged(int index);

  void onTimerTimeout();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_CONTROL_PANEL_FRAME_H
