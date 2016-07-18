// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/avatar_button.h"

#include <QImage>
#include <QPainter>
#include <QPaintEvent>

#include "base/file_util.h"

namespace ui {

namespace {

const int kIconSize = 80;

}  // namespace

AvatarButton::AvatarButton(const QString& icon, QWidget* parent)
  : FlatButton(parent),
    icon_(icon) {
//  Q_ASSERT(!icon.isEmpty());
  this->setObjectName(QStringLiteral("avatar_button"));

  this->setFixedSize(kIconSize, kIconSize);
  this->setStyleSheet(base::ReadTextFileContent(":/styles/avatar_button.css"));
}

void AvatarButton::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter painter(this);

  QRect ellipse_rect((width() - kIconSize) / 2, (height() - kIconSize) / 2,
                     kIconSize, kIconSize);
  QPainterPath path;
  path.addEllipse(ellipse_rect);
  painter.setRenderHint(QPainter::Antialiasing);
  // Scale image.
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.setClipPath(path);

  QImage image(icon_);
  painter.drawImage(ellipse_rect, image);

  painter.end();
}

}  // namespace ui