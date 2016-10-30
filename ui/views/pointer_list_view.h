// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_POINTER_LIST_VIEW_H
#define INSTALLER_UI_DELEGATES_POINTER_LIST_VIEW_H

#include <QListView>

namespace installer {

// Change cursor shape when mouse hovers onto item in list view.
class PointerListView : public QListView {
  Q_OBJECT

 public:
  explicit PointerListView(QWidget* parent = nullptr);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_POINTER_LIST_VIEW_H

