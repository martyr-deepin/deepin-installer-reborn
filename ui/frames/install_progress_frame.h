// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>

namespace service {
class HooksManager;
}  // namespace service

namespace ui {

// Displays when system is being installed to disk.
// A progress bar is shown at bottom of page.
class InstallProgressFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);
  ~InstallProgressFrame();

  enum class State {
    Failed,
    Finished,
    InProcess,
  };

  // Returns true is installation process failed.
  bool failed() const { return failed_; }

  // Returns error message when installation process failed.
  QString error_message() const { return error_message_; }

  // Initialize hooks manager.
  void initHooks();

 signals:
  // Emitted when installation finished or failed.
  // Call state() to check its status.
  void finished();

 private:
  void initConnections();
  void initUI();

  bool failed_;
  QString error_message_;

  bool hooks_inited_;

  service::HooksManager* hooks_manager_ = nullptr;

 private slots:
  void onErrorOccurred(const QString& msg);
  void onInstallProgressUpdated(int progress);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
