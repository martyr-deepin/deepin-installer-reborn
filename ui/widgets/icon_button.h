// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ICON_BUTTON_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ICON_BUTTON_H

#include "ui/widgets/flat_button.h"

#include <QIcon>
class QMouseEvent;

namespace ui {

// IconButton displays different icons in a flat button at different states.
class IconButton : public FlatButton {
  Q_OBJECT

 public:
  IconButton(const QString& normal_icon,
             const QString& hover_icon,
             const QString& active_icon,
             int button_width,
             int button_height,
             QWidget* parent = nullptr);

 protected:
  // Override these methods to change icons based on mouse action.
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  QIcon normal_icon_;
  QIcon hover_icon_;
  QIcon active_icon_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_ICON_BUTTON_H
