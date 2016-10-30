// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_FRAMELESS_LIST_VIEW_H
#define INSTALLER_FRAMELESS_LIST_VIEW_H

#include <QListView>

namespace installer {

// Provides a frameless list view, with round corners and without scroll bars.
class FramelessListView : public QListView {
  Q_OBJECT

 public:
  explicit FramelessListView(QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_FRAMELESS_LIST_VIEW_H
