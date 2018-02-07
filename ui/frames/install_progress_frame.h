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

#ifndef INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>
class QLabel;
class QProgressBar;
class QPropertyAnimation;
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
  Q_PROPERTY(int progress READ progress WRITE setProgress);

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);
  ~InstallProgressFrame();

  // Returns true is installation process failed.
  bool failed() const { return failed_; }

  int progress() const { return progress_; }

  // Show slide now.
  void startSlide();

 public slots:
  // Run hooks when partition job is done
  void runHooks(bool ok);

  void setProgress(int progress);

  // Update progress value with a QTimer object.
  void simulate();

 signals:
  // Emitted when installation finished or failed.
  // Call state() to check its status.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Update value of progress bar to |progress| and update tooltip position.
  void updateProgressBar(int progress);

  bool failed_;

  // Progress value.
  int progress_;

  HooksManager* hooks_manager_ = nullptr;
  QThread* hooks_manager_thread_ = nullptr;

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  InstallProgressSlideFrame* slide_frame_ = nullptr;
  QLabel* tooltip_label_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;

  QPropertyAnimation* progress_animation_ = nullptr;

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
