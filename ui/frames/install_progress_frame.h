// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>
class QLabel;
class QProgressBar;
class QThread;

namespace installer {

class HooksManager;
class InstallProgressBar;
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

  // Returns true is installation process failed.
  bool failed() const { return failed_; }

 public slots:
  // Run hooks when partition job is done
  void runHooks(bool ok);

  // Choose slide package based on |locale|.
  void updateLanguage(const QString& locale);

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

  InstallProgressSlideFrame* slide_frame_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;

 private slots:
  // Handles error state
  void onErrorOccurred();

  // Handles successful installation.
  void onSucceeded();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
