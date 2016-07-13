// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TITLE_LABEL_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TITLE_LABEL_H

#include <QLabel>

namespace ui {

// TitleLabel is used as header content of each frame page.
class TitleLabel : public QLabel {
  Q_OBJECT

 public:
  TitleLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_TITLE_LABEL_H
