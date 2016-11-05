// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H
#define INSTALLER_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H

#include <QFrame>

namespace installer {

class NavButton;

// This page is displayed when system is running in a virtual machine.
class VirtualMachineFrame : public QFrame {
  Q_OBJECT
  
 public:
  explicit VirtualMachineFrame(QWidget* parent = nullptr);

 signals:
  // This signal is emitted when continue-button is clicked.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* next_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_VIRTUAL_MACHINE_FRAME_H
