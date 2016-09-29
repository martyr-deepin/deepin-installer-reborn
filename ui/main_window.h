// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MAIN_WINDOW_H
#define INSTALLER_UI_MAIN_WINDOW_H

#include <QWidget>
#include <QHash>
class QLabel;
class QResizeEvent;
class QShortcut;
class QStackedLayout;

namespace installer {

class IconButton;
class PageIndicator;
class ConfirmQuitFrame;
class DiskSpaceInsufficientFrame;
class InstallFailedFrame;
class InstallProgressFrame;
class InstallSuccessFrame;
class LogViewerFrame;
class PartitionFrame;
class PartitionTableWarningFrame;
class SelectLanguageFrame;
class SystemInfoFrame;
class VirtualMachineFrame;

// MainWindow is a fullscreen window.
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

  // Show fullscreen. Do not call showFullscreen(), which does not work well
  // with deepin-wm.
  void fullscreen();

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
    PartitionTableWarningId,
    SelectLanguageId,
    SystemInfoId,
    VirtualMachineId,
  };

  void initConnections();
  void initPages();
  void initUI();
  void registerShortcut();
  // Switch frame page based on name.
  void setCurrentPage(PageId page_id);
  void updateBackground();

  QLabel* background_label_ = nullptr;
  IconButton* close_button_ = nullptr;
  PageIndicator* page_indicator_ = nullptr;
  // All of frame pages are stored in this layout.
  // And they are referenced by id in |pages_|.
  QStackedLayout* stacked_layout_ = nullptr;

  ConfirmQuitFrame* confirm_quit_frame_ = nullptr;
  DiskSpaceInsufficientFrame* disk_space_insufficient_frame_ = nullptr;
  InstallFailedFrame* install_failed_frame_ = nullptr;
  InstallProgressFrame* install_progress_frame_ = nullptr;
  InstallSuccessFrame* install_success_frame_ = nullptr;
  LogViewerFrame* log_viewer_frame_ = nullptr;
  PartitionFrame* partition_frame_ = nullptr;
  PartitionTableWarningFrame* partition_table_warning_frame_ = nullptr;
  SelectLanguageFrame* select_language_frame_ = nullptr;
  SystemInfoFrame* system_info_frame_ = nullptr;
  VirtualMachineFrame* virtual_machine_frame_ = nullptr;

  // To store frame pages, page_name => page_id.
  QHash<PageId, int> pages_;

  // Keep previous page id. It is used by ConfirmQuitPage.
  PageId prev_page_;
  PageId current_page_;

  QShortcut* log_viewer_shortcut_ = nullptr;

 private slots:
  // Show ConfirmQuitFrame when close_button_ is clicked.
  void onCloseButtonClicked();

  void goNextPage();
  void rebootSystem();
  void shutdownSystem();
};

}  // namespace installer

#endif  // INSTALLER_UI_MAIN_WINDOW_H
