// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>
class QLabel;
class QThread;

namespace installer {

class HooksManager;
class InstallProgressSlideFrame;

// Displays when system is being installed to disk.
// A progress bar is shown at bottom of page.
class InstallProgressFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);
  ~InstallProgressFrame();

  // Show slide now
  void startSlide();

  enum class State {
    Failed,
    Finished,
    InProcess,
  };

  // Returns true is installation process failed.
  bool failed() const { return failed_; }

 signals:
  // Emitted when installation finished or failed.
  // Call state() to check its status.
  void finished();

 private:
  void initConnections();
  void initUI();

  bool failed_;

  HooksManager* hooks_manager_ = nullptr;
  QThread* hooks_manager_thread_ = nullptr;

  QLabel* progress_label_ = nullptr;
  InstallProgressSlideFrame* slide_frame_ = nullptr;

 private slots:
  void onErrorOccurred();
  void onInstallProgressUpdated(int progress);

  void onPartitionDone(bool ok);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
