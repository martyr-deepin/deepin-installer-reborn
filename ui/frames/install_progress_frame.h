// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>

namespace ui {

// Displays when system is being installed to disk.
// A progress bar is shown at bottom of page.
class InstallProgressFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);

  enum class State {
    Failed,
    Finished,
    InProcess,
  };

  State state() const { return state_; };

 signals:
  // Emitted when installation finished or failed.
  // Call state() to check its status.
  void finished();

 private:
  void initConnections();
  void initUI();

  State state_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
