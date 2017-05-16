// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SIMPLE_DISK_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SIMPLE_DISK_FRAME_H

#include <QFrame>
class QAbstractButton;
class QButtonGroup;
class QGridLayout;
class QLabel;
class QShowEvent;

namespace installer {

class SimplePartitionDelegate;

// Only displays device disks.
class SimpleDiskFrame : public QFrame {
  Q_OBJECT

 public:
  SimpleDiskFrame(SimplePartitionDelegate* delegate, QWidget* parent = nullptr);

  bool validate() const;

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Repaint partition list.
  void repaintDevices();

  // Show install_tip at bottom of |button|.
  void showInstallTip(QAbstractButton* button);

  // Show install_tip at bottom of |button|.
//  void showInstallTip(QAbstractButton* button);

  SimplePartitionDelegate* delegate_ = nullptr;
  QButtonGroup* button_group_ = nullptr;
  QFrame* install_tip_ = nullptr;
  QLabel* tip_label_ = nullptr;
  QGridLayout* grid_layout_ = nullptr;
  QFrame* grid_wrapper_ = nullptr;

 private slots:
  void onDeviceRefreshed();
  void onPartitionButtonToggled(QAbstractButton* button,
                                bool checked);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SIMPLE_DISK_FRAME_H
