// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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

  QLabel* background_label_ = nullptr;
  SystemInfoFrame* system_info_frame_ = nullptr;
  FirstBootLoadingFrame* loading_frame_ = nullptr;
  QStackedLayout* stacked_layout_ = nullptr;

  QThread* hook_worker_thread_ = nullptr;
  FirstBootHookWorker* hook_worker_ = nullptr;

  // Shortcut used to switch mirror modes.
  GlobalShortcut* monitor_mode_shortcut_ = nullptr;
  MultiHeadManager* multi_head_manager_ = nullptr;

 private slots:
  // Handles result of hook worker.
  void onHookFinished(bool ok);

  void onPrimaryScreenChanged(const QRect& geometry);

  // Run "first_boot_setup.sh" after system_info_frame_ is finished.
  void onSystemInfoFinished();
};

}  // namespace installer

#endif  // INSTALLER_UI_SETUP_WINDOW_H
