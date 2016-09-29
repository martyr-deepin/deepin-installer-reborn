// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_TITLE_LABEL_H
#define INSTALLER_UI_WIDGETS_TITLE_LABEL_H

#include <QLabel>

namespace installer {

// TitleLabel is used as header content of each frame page.
class TitleLabel : public QLabel {
  Q_OBJECT

 public:
  TitleLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_TITLE_LABEL_H
