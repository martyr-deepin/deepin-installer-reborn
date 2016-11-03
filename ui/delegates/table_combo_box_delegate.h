// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_TABLE_COMBO_BOX_DELEGATE_H
#define INSTALLER_UI_DELEGATES_TABLE_COMBO_BOX_DELEGATE_H

#include <QStyledItemDelegate>
class QPainter;

namespace installer {

// Delegate used in TableComboBox
class TableComboBoxDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit TableComboBoxDelegate(QWidget* parent = nullptr);

 protected:
  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_TABLE_COMBO_BOX_DELEGATE_H
