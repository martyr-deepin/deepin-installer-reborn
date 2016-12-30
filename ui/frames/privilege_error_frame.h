// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H
#define INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H

#include <QFrame>
#include <QPushButton>

namespace installer {

// Display this page when root privilege is not guaranteed.
class PrivilegeErrorFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PrivilegeErrorFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when exit_button_ is clicked.
  void finished();

 private:
  void initConnection();
  void initUI();

  QPushButton* exit_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PRIVILEGE_ERROR_FRAME_H
