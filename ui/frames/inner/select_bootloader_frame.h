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

#ifndef INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H

#include <QFrame>

#include "partman/device.h"

namespace installer {

class CommentLabel;
class FramelessListView;
class NavButton;
class BootloaderListModel;
class TitleLabel;

// Displays a window to select available boot path.
// This frame read recommended bootloader path, and notifies any changes to
// PartitionDelegate and AdvancedPartitionFrame.
// It refreshes available partition list for bootloader when mount-point list
// is updated in PartitionDelegate.
class SelectBootloaderFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectBootloaderFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when currently used bootloader path is updated.
  void bootloaderUpdated(const QString& bootloader_path, bool simple_mode);

  void deviceRefreshed(const DeviceList& devices);

  // Emitted when back-button is clicked.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  FramelessListView* list_view_ = nullptr;
  BootloaderListModel* list_model_ = nullptr;
  NavButton* back_button_ = nullptr;

 private slots:
  // Update partition list
  void onModelChanged();

  // Emit signals when new bootloader path is selected in list-view.
  void onPartitionListViewSelected(const QModelIndex& current,
                                   const QModelIndex& previous);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
