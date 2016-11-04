// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/views/pointer_list_view.h"

#include <QMouseEvent>

#include "ui/utils/xutil.h"

namespace installer {

PointerListView::PointerListView(QWidget* parent) : QListView(parent) {
  this->setObjectName("pointer_list_view");
  this->setMouseTracking(true);
}

void PointerListView::mouseMoveEvent(QMouseEvent* event) {
  QAbstractItemModel* model = this->model();
  if (model) {
    const QModelIndex index = this->indexAt(event->pos());
    // Change cursor shape to "Hand" when mouse enters list view item.
    if (index.isValid()) {
      utils::SetCursorShape(this, XC_hand1);
    } else {
      utils::ResetCursorShape(this);
    }
  }
  QListView::mouseMoveEvent(event);
}

}  // namespace installer