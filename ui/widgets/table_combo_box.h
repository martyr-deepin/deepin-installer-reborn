// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_TABLE_COMBO_BOX_H
#define INSTALLER_UI_WIDGETS_TABLE_COMBO_BOX_H

#include <QComboBox>

namespace installer {

// Combo box used in grid layout.
class TableComboBox : public QComboBox {
  Q_OBJECT

 public:
  explicit TableComboBox(QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_TABLE_COMBO_BOX_H
