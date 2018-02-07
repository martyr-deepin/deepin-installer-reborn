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

#include "ui/views/frameless_list_view.h"

#include "base/file_util.h"
#include "ui/delegates/frameless_list_delegate.h"

namespace installer {

FramelessListView::FramelessListView(QWidget* parent)
    : QListView(parent) {
  this->setObjectName("frameless_list_view");

  FramelessItemDelegate* delegate = new FramelessItemDelegate(this);
  this->setItemDelegate(delegate);

  this->setStyleSheet(ReadFile(":/styles/frameless_list_view.css"));
  QSizePolicy list_policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  list_policy.setHorizontalStretch(1);
  list_policy.setVerticalStretch(1);
  this->setSizePolicy(list_policy);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QPalette list_palette = this->palette();
  list_palette.setBrush(QPalette::Base, Qt::transparent);
  this->setPalette(list_palette);
  this->setSelectionMode(QAbstractItemView::SingleSelection);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setFixedWidth(468);
  this->setSelectionRectVisible(false);
  this->setUniformItemSizes(true);
}

}  // namespace installer