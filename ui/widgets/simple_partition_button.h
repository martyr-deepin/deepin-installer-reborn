// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H

#include "ui/widgets/flat_button.h"

class QLabel;

#include "partman/partition.h"

namespace ui {

class SimplePartitionButton : public FlatButton {
  Q_OBJECT

 public:
  SimplePartitionButton(const partman::Partition& partition,
                        QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  const partman::Partition& partition_;

  QLabel* fs_label_ = nullptr;

 private slots:
  void onButtonToggled();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
