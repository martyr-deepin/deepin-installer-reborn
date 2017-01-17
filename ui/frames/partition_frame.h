// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_PARTITION_FRAME_H

#include <QFrame>
class QStackedLayout;

#include "partman/partition.h"

namespace installer {

class AdvancedPartitionFrame;
class CommentLabel;
class EditPartitionFrame;
class NavButton;
class NewPartitionFrame;
class PartitionDelegate;
class PartitionLoadingFrame;
class PointerButton;
class PrepareInstallFrame;
class SelectBootloaderFrame;
class SimplePartitionFrame;
class TitleLabel;

// Handles partition operations.
class PartitionFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PartitionFrame(QWidget* parent = nullptr);

  void autoPart();

 signals:
  // Emitted when partition edition is done.
  void finished();

  // Emitted when partition job is done.
  void autoPartDone(bool ok);
  void manualPartDone(bool ok);

 public slots:
  // Notify delegate to scan devices.
  void scanDevices() const;

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  AdvancedPartitionFrame* advanced_partition_frame_ = nullptr;
  EditPartitionFrame* edit_partition_frame_ = nullptr;
  NewPartitionFrame* new_partition_frame_ = nullptr;
  PartitionLoadingFrame* partition_loading_frame_ = nullptr;
  PrepareInstallFrame* prepare_install_frame_ = nullptr;
  SelectBootloaderFrame* select_bootloader_frame_ = nullptr;
  SimplePartitionFrame* simple_partition_frame_ = nullptr;

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  QFrame* main_frame_ = nullptr;
  QStackedLayout* partition_stacked_layout_ = nullptr;
  QStackedLayout* main_layout_ = nullptr;
  PointerButton* simple_frame_button_ = nullptr;
  PointerButton* advanced_frame_button_ = nullptr;
  NavButton* next_button_ = nullptr;

  PartitionDelegate* delegate_ = nullptr;

 private slots:
  void onSimpleFrameButtonToggled();
  void onAdvancedFrameButtonToggled();
  void onNextButtonClicked();

  void showEditPartitionFrame(const Partition& partition);
  void showMainFrame();
  void showNewPartitionFrame(const Partition& partition);
  void showSelectBootloaderFrame();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PARTITION_FRAME_H
