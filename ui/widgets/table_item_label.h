// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_ITEM_LABEL_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_ITEM_LABEL_H

#include <QLabel>

namespace ui {

// These type of labels are used in grid layout.
// Text is right-aligned.
class TableItemLabel : public QLabel {
  Q_OBJECT

 public:
  TableItemLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TABLE_ITEM_LABEL_H
