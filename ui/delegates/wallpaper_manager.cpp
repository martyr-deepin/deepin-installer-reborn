// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/wallpaper_manager.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include "ui/widgets/wallpaper_item.h"

namespace installer {

WallpaperManager::WallpaperManager(QObject* parent)
    : QObject(parent),
      wallpaper_items_() {
  this->setObjectName(QStringLiteral("wallpaper_manager"));

  desktop_widget_ = QApplication::desktop();
  this->initConnections();
}

void WallpaperManager::updateWallpaper() {
  qDebug() << "WallpaperManager() updateWallpaper";
  // Clear old wallpaper items
  for (WallpaperItem* item : wallpaper_items_) {
    delete item;
    item = nullptr;
  }
  wallpaper_items_.clear();

  const int screen_count = desktop_widget_->screenCount();
  for (int i = 0; i < screen_count; i++) {
    const QRect geometry = desktop_widget_->screenGeometry(i);
    qDebug() << "Add wallpaper:" << i << geometry;
    WallpaperItem* item = new WallpaperItem(geometry);
    wallpaper_items_.append(item);
    item->show();
  }
}

void WallpaperManager::initConnections() {
  connect(desktop_widget_, &QDesktopWidget::primaryScreenChanged,
          this, &WallpaperManager::onPrimaryScreenChanged);
  connect(desktop_widget_, &QDesktopWidget::screenCountChanged,
          this, &WallpaperManager::onScreenCountChanged);
}

void WallpaperManager::onPrimaryScreenChanged() {
  qDebug() << "Primary screen changed.";
  this->updateWallpaper();
}

void WallpaperManager::onScreenCountChanged() {
  qDebug() << "Screen count changed.";
  this->updateWallpaper();
}

}  // namespace installer