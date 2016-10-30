// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H
#define INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>

namespace installer {

// ItemDelegate for FramelessListView.
class FramelessItemDelegate : public QItemDelegate {
  Q_OBJECT

 public:
  explicit FramelessItemDelegate(QObject* parent = nullptr);

  // Modifies item language list item size.
//  QSize sizeHint(const QStyleOptionViewItem& option,
//                 const QModelIndex& index) const override;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H
