// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H
#define INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H

#include <QStyledItemDelegate>
class QPainter;

namespace installer {

// ItemDelegate for FramelessListView.
// This class overrides default styles in QStyledItemDelegate.
// And stylesheets defined in :/styles/frameless_list_view.css are useless
// for list items.
class FramelessItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit FramelessItemDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_LANGUAGE_LIST_DELEGATE_H
