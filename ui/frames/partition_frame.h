// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H

#include <QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedLayout>

class QPushButton;
class QStackedLayout;

namespace service {
class PartitionManager;
}  // namespace service

namespace ui {

class AdvancedPartitionFrame;
class EditPartitionFrame;
class NavButton;
class NewPartitionFrame;
class PrepareInstallFrame;
class SelectBootloaderFrame;
class SimplePartitionFrame;

// Handles partition operations.
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

  AdvancedPartitionFrame* advanced_partition_frame_ = nullptr;
  EditPartitionFrame* edit_partition_frame_ = nullptr;
  NewPartitionFrame* new_partition_frame_ = nullptr;
  PrepareInstallFrame* prepare_install_frame_ = nullptr;
  SelectBootloaderFrame* select_bootloader_frame_ = nullptr;
  SimplePartitionFrame* simple_partition_frame_ = nullptr;

  QFrame* main_frame_ = nullptr;
  QStackedLayout* partition_stacked_layout_ = nullptr;
  QStackedLayout* main_layout_ = nullptr;
  QPushButton* simple_frame_button_ = nullptr;
  QPushButton* advanced_frame_button_ = nullptr;
  NavButton* next_button_ = nullptr;

  service::PartitionManager* partition_manager_ = nullptr;

 private slots:
  void onSimpleFrameButtonToggled();
  void onAdvancedFrameButtonToggled();
  void onNextButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_PARTITION_FRAME_H
