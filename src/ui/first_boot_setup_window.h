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

#ifndef INSTALLER_UI_SETUP_WINDOW_H
#define INSTALLER_UI_SETUP_WINDOW_H

#include <QFrame>
class QLabel;
class QResizeEvent;
class QStackedLayout;
class QThread;

class GlobalShortcut;

namespace installer {

class FirstBootLoadingFrame;
class FirstBootHookWorker;
class MultiHeadManager;
class SystemInfoFrame;
class TimezoneFrame;
class LanguageFrame;
class NetworkFrame;
class ControlPlatformFrame;

// Main window of deepin_installer_first_boot.
class FirstBootSetupWindow : public QFrame {
  Q_OBJECT

 public:
  explicit FirstBootSetupWindow(QWidget* parent = nullptr);
  ~FirstBootSetupWindow();

  // Show fullscreen.
  void fullscreen();

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  void initConnections();
  void initUI();

  void registerShortcut();
  void updateBackground();

  QLabel*                background_label_    = nullptr;
  SystemInfoFrame*       system_info_frame_   = nullptr;
  TimezoneFrame*         timezone_frame_      = nullptr;
  FirstBootLoadingFrame* loading_frame_       = nullptr;
  LanguageFrame*         language_frame_      = nullptr;
  NetworkFrame*          network_frame_       = nullptr;
  ControlPlatformFrame*  control_platform_frame_ = nullptr;
  QStackedLayout*        stacked_layout_      = nullptr;

  QThread*             hook_worker_thread_ = nullptr;
  FirstBootHookWorker* hook_worker_        = nullptr;

  // Shortcut used to switch mirror modes.
  GlobalShortcut*   monitor_mode_shortcut_ = nullptr;
  MultiHeadManager* multi_head_manager_    = nullptr;

  private slots:
  // Handles result of hook worker.
  void onHookFinished(bool ok);

  void onPrimaryScreenChanged(const QRect& geometry);

  void onLanguageSelected();
  void onSystemInfoFinished();

  void onNetworkFinished();

  void onControlPlatformFinished();

  // Run "first_boot_setup.sh" after system_info_frame_ is finished.
  void onTimezoneFinished();
};

}  // namespace installer

#endif  // INSTALLER_UI_SETUP_WINDOW_H
