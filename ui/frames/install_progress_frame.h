// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>
class QLabel;
class QProgressBar;
class QThread;
class QTimer;

namespace installer {

class CommentLabel;
class HooksManager;
class InstallProgressBar;
class InstallProgressSlideFrame;
class TitleLabel;

// Displays when system is being installed to disk.
// A progress bar is shown at bottom of page.
class InstallProgressFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);
  ~InstallProgressFrame();

  // Show slide now.
  void startSlide();

  // Returns true is installation process failed.
  bool failed() const { return failed_; }

 public slots:
  // Run hooks when partition job is done
  void runHooks(bool ok);

  // Update progress value with a qtimer object.
  void simulate();

  // Choose slide package based on |locale|.
  void updateLanguage(const QString& locale);

 signals:
  // Emitted when installation finished or failed.
  // Call state() to check its status.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  bool failed_;

  HooksManager* hooks_manager_ = nullptr;
  QThread* hooks_manager_thread_ = nullptr;

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  InstallProgressSlideFrame* slide_frame_ = nullptr;
  QLabel* tooltip_label_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;

  QTimer* simulation_timer_ = nullptr;

 private slots:
  // Handles error state
  void onHooksErrorOccurred();

  // Handles successful installation.
  void onHooksFinished();

  void onProgressUpdate(int progress);

  void onRetainingTimerTimeout();

  void onSimulationTimerTimeout();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
