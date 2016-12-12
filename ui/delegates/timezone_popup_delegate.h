// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_TIMEZONE_POPUP_DELEGATE_H
#define INSTALLER_UI_DELEGATES_TIMEZONE_POPUP_DELEGATE_H

#include <QStyledItemDelegate>

namespace installer {

// Draw zone item in popup window of timezone map.
// Both first row and last row have rounded corners.
class TimezonePopupDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit TimezonePopupDelegate(QWidget* parent = nullptr);

 protected:
  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

}  // namespace installer

#endif //INSTALLER_UI_DELEGATES_TIMEZONE_POPUP_DELEGATE_H
