// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/wallpaper_item.h"

#include <QDebug>

#include "service/settings_manager.h"
#include "ui/utils/xutil.h"

namespace installer {

WallpaperItem::WallpaperItem(const QRect& background_geometry,
                             QWidget* parent) :
    QLabel(parent),
    background_geometry_(background_geometry) {
  this->setObjectName(QStringLiteral("wallpaper_item"));

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
  qDebug() << "move to:" << background_geometry_;
}

}  // namespace installer