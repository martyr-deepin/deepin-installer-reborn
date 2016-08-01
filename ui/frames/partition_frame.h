// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H

#include <QFrame>

namespace service {
class PartitionManager;
}  // namespace service

namespace ui {

class NavButton;

// Handles partion operations.
class PartitionFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionFrame(QWidget* parent = nullptr);

  void autoPart();

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* next_button_ = nullptr;
  service::PartitionManager* partition_manager_ = nullptr;

 private slots:
  void onNextButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H
