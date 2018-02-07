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

#include "ui/widgets/wallpaper_item.h"

#include <QDebug>

#include "service/settings_manager.h"
#include "ui/utils/xutil.h"

namespace installer {

WallpaperItem::WallpaperItem(const QRect& background_geometry,
                             QWidget* parent) :
    QLabel(parent),
    background_geometry_(background_geometry) {
  this->setObjectName("wallpaper_item");

  this->initUI();
}

void WallpaperItem::initUI() {
  const QSize window_size = background_geometry_.size();
  this->setFixedSize(window_size);
  this->move(background_geometry_.topLeft());
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  utils::SetWindowTypeHint(this, utils::X11WindowType::Desktop);
  utils::SkipTaskbarPager(this);

  const QString image_path = GetWindowBackground();
  const QPixmap orig_pixmap(image_path);
  const QPixmap pixmap = orig_pixmap.scaled(window_size,
                                            Qt::KeepAspectRatioByExpanding);
  this->setPixmap(pixmap);
}

}  // namespace installer