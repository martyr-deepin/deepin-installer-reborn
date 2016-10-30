// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/views/frameless_list_view.h"

#include "base/file_util.h"
#include "ui/delegates/frameless_list_delegate.h"

namespace installer {

FramelessListView::FramelessListView(QWidget* parent)
    : QListView(parent) {
  this->setObjectName(QStringLiteral("frameless_list_view"));

//  FramelessItemDelegate* delegate = new FramelessItemDelegate(this);
//  this->setItemDelegate(delegate);

  this->setStyleSheet(ReadTextFileContent(":/styles/frameless_list_view.css"));
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