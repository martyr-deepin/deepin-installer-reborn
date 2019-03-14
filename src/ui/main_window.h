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

#ifndef INSTALLER_UI_MAIN_WINDOW_H
#define INSTALLER_UI_MAIN_WINDOW_H

#include <QWidget>
#include <QHash>
class QLabel;
class QPushButton;
class QResizeEvent;
class QShortcut;
class QStackedLayout;

class GlobalShortcut;

namespace installer {

class ConfirmQuitFrame;
class ControlPanelFrame;
class DiskSpaceInsufficientFrame;
class InstallFailedFrame;
class InstallProgressFrame;
class InstallSuccessFrame;
class MultiHeadManager;
class PageIndicator;
class PartitionFrame;
class PartitionTableWarningFrame;
class PrivilegeErrorFrame;
class LanguageFrame;
class SystemInfoFrame;
class TimezoneFrame;
class VirtualMachineFrame;

// MainWindow is a fullscreen window of deepin-installer.
// All of ui frames are placed in MainWindow.
// It does following jobs:
//   * draw window background with a blur image;
//   * handles keyboard shortcut like Ctrl+P and Ctrl+L;
//   * handles window navigation (by providing a content area);
//   * quit / abort installation process.
class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow();

  // Show fullscreen.
  void fullscreen();

  // Notify background thread to scan disk devices if needed.
  // And read current timezone.
  void scanDevicesAndTimezone();

  // Enable auto-install mode.
  void setEnableAutoInstall(bool auto_install);

  // Set filepath to which log file will be backup.
  void setLogFile(const QString& log_file);

 protected:
  // Move close button to appropriate position when window is resized.
  void resizeEvent(QResizeEvent* event) override;

private:
  enum PageId {
      NullId,  // page not set.

      ConfirmQuitId,
      DiskSpaceInsufficientId,
      InstallFailedId,
      InstallProgressId,
      InstallSuccessId,
      PartitionId,
      PrivilegeErrorId,
      SelectLanguageId,
      SystemInfoId,
      TimezoneId,
      VirtualMachineId,
      UserAgreementId,
  };

  void initConnections();
  void initPages();
  void initUI();
  void registerShortcut();

  // Copy log file if needed.
  void saveLogFile();

  // Switch frame page based on name.
  void setCurrentPage(PageId page_id);

  void updateBackground();

  QLabel* background_label_ = nullptr;
  QPushButton* close_button_ = nullptr;
  PageIndicator* page_indicator_ = nullptr;
  // All of frame pages are stored in this layout.
  // And they are referenced by id in |pages_|.
  QStackedLayout* stacked_layout_ = nullptr;

  ConfirmQuitFrame* confirm_quit_frame_ = nullptr;
  ControlPanelFrame* control_panel_frame_ = nullptr;
  DiskSpaceInsufficientFrame* disk_space_insufficient_frame_ = nullptr;
  InstallFailedFrame* install_failed_frame_ = nullptr;
  InstallProgressFrame* install_progress_frame_ = nullptr;
  InstallSuccessFrame* install_success_frame_ = nullptr;
  PartitionFrame* partition_frame_ = nullptr;
  PrivilegeErrorFrame* privilege_error_frame_ = nullptr;
  LanguageFrame* select_language_frame_ = nullptr;
  SystemInfoFrame* system_info_frame_ = nullptr;
  TimezoneFrame* timezone_frame_ = nullptr;
  VirtualMachineFrame* virtual_machine_frame_ = nullptr;
  MultiHeadManager* multi_head_manager_ = nullptr;

  // To store frame pages, page_name => page_id.
  QHash<PageId, int> pages_;

  // Keep previous page id. It is used by ConfirmQuitPage.
  PageId prev_page_;
  PageId current_page_;

  // Shortcut used to toggle visibility of log-viewer.
  QShortcut* control_panel_shortcut_ = nullptr;

  // Shortcut used to switch mirror modes.
  GlobalShortcut* monitor_mode_shortcut_ = nullptr;

  // Shortcut for screen brightness.
  QShortcut* brightness_increase_shortcut_ = nullptr;
  QShortcut* brithtness_decrease_shortcut_ = nullptr;

  QString log_file_;
  bool auto_install_;

 private slots:
  // Go next page when current page index is changed in ControlPanelFrame.
  void onCurrentPageChanged(int index);

  // Show ConfirmQuitFrame when close_button_ is clicked.
  void onCloseButtonClicked();

  // Move main window to primary screen when it is changed to |geometry|.
  void onPrimaryScreenChanged(const QRect& geometry);

  void goNextPage();
  void rebootSystem();
  void shutdownSystem();
};

}  // namespace installer

#endif  // INSTALLER_UI_MAIN_WINDOW_H
