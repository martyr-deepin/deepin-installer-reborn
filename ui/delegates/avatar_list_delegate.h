// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_AVATAR_LIST_DELEGATE_H
#define INSTALLER_UI_DELEGATES_AVATAR_LIST_DELEGATE_H

#include <QStyledItemDelegate>

namespace installer {

// Draw AvatarButton, used in ListView of SystemInfoAvatarFrame.
class AvatarListDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  explicit AvatarListDelegate(QWidget* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_AVATAR_LIST_DELEGATE_H
