// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_SETUP_WINDOW_H
#define INSTALLER_UI_SETUP_WINDOW_H

#include <QFrame>
class QLabel;
class QResizeEvent;
class QStackedLayout;

namespace installer {

class SystemInfoFrame;

// Main window of deepin_installer_reboot_setup.
class SetupWindow : public QFrame {
  Q_OBJECT

 public:
  explicit SetupWindow(QWidget* parent = nullptr);

  // Show fullscreen.
  void fullscreen();

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  void initConnections();
  void initUI();

  void updateBackground();

  QLabel* background_label_ = nullptr;
  SystemInfoFrame* system_info_frame_ = nullptr;
  QStackedLayout* stacked_layout_ = nullptr;

 private slots:
  // Run "setup_sys_info.sh" after system_info_frame_ is finished.
  void onSystemInfoFinished();
};

}  // namespace installer

#endif  // INSTALLER_UI_SETUP_WINDOW_H
