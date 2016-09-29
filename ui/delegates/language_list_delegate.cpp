// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "language_list_delegate.h"

namespace installer {

LanguageListDelegate::LanguageListDelegate(QObject* parent)
    : QItemDelegate(parent) {
  this->setObjectName(QStringLiteral("language_list_delegate"));
}

QSize LanguageListDelegate::sizeHint(const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
  Q_UNUSED(option);
  Q_UNUSED(index);
  return QSize(280, 38);
}

}  // namespace installer