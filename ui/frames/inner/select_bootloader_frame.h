// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class FramelessListView;
class NavButton;
class PartitionDelegate;
class PartitionListModel;
class TitleLabel;

// Displays a window to select available boot path.
// This frame read recommended bootloader path, and notifies any changes to
// PartitionDelegate and AdvancedPartitionFrame.
// It refreshes available partition list for bootloader when mount-point list
// is updated in PartitionDelegate.
class SelectBootloaderFrame : public QFrame {
  Q_OBJECT

 public:
  SelectBootloaderFrame(PartitionDelegate* delegate, QWidget* parent = nullptr);

 signals:
  // Emitted when currently used bootloader path is updated.
  void bootloaderUpdated(const QString& bootloader_path);

  // Emitted when back-button is clicked.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  PartitionDelegate* delegate_ = nullptr;
  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  FramelessListView* list_view_ = nullptr;
  PartitionListModel* list_model_ = nullptr;
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
