// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_INSTALL_PROGRESS_TIP_H
#define INSTALLER_UI_WIDGETS_INSTALL_PROGRESS_TIP_H

#include <QLabel>
class QPaintEvent;

namespace installer {

// Draw install progress value in InstallProgressFrame with round corner.
class InstallProgressTip : public QLabel {
  Q_OBJECT

 public:
  explicit InstallProgressTip(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
};

}  // namespace instaler

#endif  // INSTALLER_UI_WIDGETS_INSTALL_PROGRESS_TIP_H
