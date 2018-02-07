/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
