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

#ifndef INSTALLER_UI_WIDGETS_TOOLTIP_PIN_H
#define INSTALLER_UI_WIDGETS_TOOLTIP_PIN_H

#include <QLabel>
class QPaintEvent;

namespace installer {

// Displays tooltip text with a sharp corner at middle of bottom edge.
class TooltipPin : public QLabel {
  Q_OBJECT

 public:
  explicit TooltipPin(QWidget* parent = nullptr);

 public slots:
  // Show tooltip and set position of pin at |point|.
  void popup(const QPoint& point);

 protected:
  void paintEvent(QPaintEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_TOOLTIP_PIN_H
