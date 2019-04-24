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

#include "ui/widgets/avatar_button.h"

#include <QPainter>
#include <QPaintEvent>

#include "base/file_util.h"

namespace installer {

namespace {

const int kIconSize = 100;

}  // namespace

AvatarButton::AvatarButton(QWidget* parent) : AvatarButton("", parent) { }

AvatarButton::AvatarButton(const QString& icon, QWidget* parent)
    : FlatButton(parent)
    , icon_(icon)
    , m_iconPixmap(icon)
{
  this->setObjectName("avatar_button");

  this->setFixedSize(kIconSize, kIconSize);
  this->setStyleSheet(ReadFile(":/styles/avatar_button.css"));
}

void AvatarButton::updateIcon(const QString& icon) {
  icon_ = icon;
  m_iconPixmap = QPixmap(icon);
  this->update();
}

void AvatarButton::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  const QRect ellipse_rect((width() - kIconSize) / 2, (height() - kIconSize) /2,
                           kIconSize, kIconSize);
  QPainterPath path;
  path.addEllipse(ellipse_rect);
  painter.setRenderHint(QPainter::Antialiasing);
  // Scale image.
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.setClipPath(path);

  painter.drawPixmap(ellipse_rect, m_iconPixmap);

  painter.end();
}

}  // namespace installer
