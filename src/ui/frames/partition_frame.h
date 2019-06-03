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

#ifndef INSTALLER_UI_FRAMES_PARTITION_FRAME_H
#define INSTALLER_UI_FRAMES_PARTITION_FRAME_H

#include <QFrame>
class QStackedLayout;

#include "partman/operation.h"
#include "partman/partition.h"

namespace installer {

class AdvancedPartitionDelegate;
class AdvancedPartitionFrame;
class CommentLabel;
class EditPartitionFrame;
class FullDiskDelegate;
class FullDiskFrame;
class NavButton;
class NewPartitionFrame;
class NewTableLoadingFrame;
class NewTableWarningFrame;
class PartitionLoadingFrame;
class PartitionModel;
class PartitionNumberLimitationFrame;
class PartitionTableWarningFrame;
class PointerButton;
class PrepareInstallFrame;
class SelectBootloaderFrame;
class SimplePartitionDelegate;
class SimplePartitionFrame;
class Full_Disk_Encrypt_frame;
class DynamicDiskWarningFrame;
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

  // Emitted when requesting reboot system.
  void reboot();

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

  // Check current partition mode is simple mode or not.
  bool isSimplePartitionMode();
  bool isFullDiskPartitionMode();

  bool isRawDevice(const QList<Device::Ptr> list);

  AdvancedPartitionFrame* advanced_partition_frame_ = nullptr;
  EditPartitionFrame* edit_partition_frame_ = nullptr;
  FullDiskFrame* full_disk_partition_frame_ = nullptr;
  NewPartitionFrame* new_partition_frame_ = nullptr;
  NewTableLoadingFrame* new_table_loading_frame_ = nullptr;
  NewTableWarningFrame* new_table_warning_frame_ = nullptr;
  PartitionLoadingFrame* partition_loading_frame_ = nullptr;
  PartitionNumberLimitationFrame* partition_number_limitation_frame_ = nullptr;
  PartitionTableWarningFrame* partition_table_warning_frame_ = nullptr;
  PrepareInstallFrame* prepare_install_frame_ = nullptr;
  SelectBootloaderFrame* select_bootloader_frame_ = nullptr;
  SimplePartitionFrame* simple_partition_frame_ = nullptr;
  Full_Disk_Encrypt_frame* full_disk_encrypt_frame_ = nullptr;
  DynamicDiskWarningFrame* dynamic_disk_warning_frame_ = nullptr;

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  QFrame* main_frame_ = nullptr;
  QStackedLayout* partition_stacked_layout_ = nullptr;
  QStackedLayout* main_layout_ = nullptr;
  PointerButton* full_disk_frame_button_ = nullptr;
  PointerButton* simple_frame_button_ = nullptr;
  PointerButton* advanced_frame_button_ = nullptr;
  NavButton* next_button_ = nullptr;

  PartitionModel* partition_model_ = nullptr;
  AdvancedPartitionDelegate* advanced_delegate_ = nullptr;
  FullDiskDelegate* full_disk_delegate_ = nullptr;
  SimplePartitionDelegate* simple_partition_delegate_ = nullptr;

 private slots:
  void onFullDiskFrameButtonToggled();
  void onSimpleFrameButtonToggled();
  void onAdvancedFrameButtonToggled();
  void onNextButtonClicked();
  void onFullDiskCryptoButtonClicked(bool encrypto);

  // Write partitioning settings to conf file and emit manualPartDone() signal.
  void onManualPartDone(bool ok, const DeviceList& devices);

  // Notify delegate to do manual part.
  void onPrepareInstallFrameFinished();

  void showEditPartitionFrame(const Partition::Ptr partition);
  void showMainFrame();
  void showNewPartitionFrame(const Partition::Ptr partition);
  void showNewTableLoadingFrame();
  void showNewTableWarningFrame(const QString& device_path);
  void showPartitionNumberLimitationFrame();
  void showPartitionTableWarningFrame(const QString& device_path);
  void showSelectBootloaderFrame();
  void showEncryptFrame();
  void showDynamicDiskFrame();
  void showPrepareInstallFrame();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PARTITION_FRAME_H
