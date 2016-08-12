// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_COMBO_BOX_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_COMBO_BOX_H

#include <QComboBox>

namespace ui {

// Combo box used in grid layout.
class TableComboBox : public QComboBox {
  Q_OBJECT

 public:
  explicit TableComboBox(QWidget* parent = nullptr);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_COMBO_BOX_H
