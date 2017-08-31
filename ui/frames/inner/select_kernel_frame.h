// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SELECT_KERNEL_FRAME_H_
#define INSTALLER_UI_FRAMES_INNER_SELECT_KERNEL_FRAME_H_

#include <QFrame>
#include <QItemSelection>
class QListView;

namespace installer {

class KernelListModel;
class NavButton;
class TitleLabel;

// Select kernel types.
class SelectKernelFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectKernelFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when next_button_ is clicked.
  void finished();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  QListView* kernel_view_ = nullptr;
  KernelListModel* kernel_model_ = nullptr;
  NavButton* next_button_ = nullptr;

 private slots:
  // Update package list in package_view_ when new package is selected in
  // kernel_view_.
  void onVersionViewSelectionChanged(const QModelIndex& current,
                                     const QModelIndex& previous);

};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SELECT_KERNEL_FRAME_H_