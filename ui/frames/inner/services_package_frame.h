// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SERVICES_PACKAGE_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SERVICES_PACKAGE_FRAME_H

#include <QFrame>
#include <QItemSelection>
class QListView;

namespace installer {

class NavButton;
class PackageListModel;
class TitleLabel;

// Display selectable services / package list.
class ServicesPackageFrame : public QFrame {
  Q_OBJECT

 public:
  explicit ServicesPackageFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when next_button_ is clicked.
  void finished();

  // Emitted when previous_button_ is clicked.
  void goPrevious();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  QListView* package_view_ = nullptr;
  PackageListModel* package_model_ = nullptr;
  NavButton* previous_button_ = nullptr;
  NavButton* next_button_ = nullptr;

 private slots:
  // Update selected package list in kernel_view_ when package selection state
  // is updated in package_view_.
  void onPackageViewSelectionChanged(const QItemSelection& selected,
                                     const QItemSelection& deselected);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SERVICES_PACKAGE_FRAME_H
