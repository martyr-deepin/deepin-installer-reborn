// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_LANGUAGE_LIST_DELEGATE_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_LANGUAGE_LIST_DELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>

namespace ui {

class LanguageListDelegate : public QItemDelegate {
  Q_OBJECT

 public:
  explicit LanguageListDelegate(QObject* parent = nullptr);

  // Modifies item language list item size.
  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_LANGUAGE_LIST_DELEGATE_H
