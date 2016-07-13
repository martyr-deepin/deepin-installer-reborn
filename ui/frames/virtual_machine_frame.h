// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H

#include <QFrame>

namespace ui {

// This page is displayed when system is running in a virtual machine.
class VirtualMachineFrame : public QFrame {
  Q_OBJECT
  
 public:
  explicit VirtualMachineFrame(QWidget* parent = nullptr);

 signals:
  // This signal is emitted when continue-button is clicked.
  void finished();

 private:
  void initUI();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H
