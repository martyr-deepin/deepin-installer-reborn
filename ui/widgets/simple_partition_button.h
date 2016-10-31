// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
#define INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H

#include "ui/widgets/pointer_button.h"
class QLabel;

#include "partman/partition.h"

namespace installer {

class SimplePartitionButton : public PointerButton {
  Q_OBJECT

 public:
  SimplePartitionButton(const Partition& partition, QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();
  const Partition& partition_;
  QLabel* os_label_ = nullptr;

 private slots:
  void onButtonToggled();
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SIMPLE_PARTITION_BUTTON_H
