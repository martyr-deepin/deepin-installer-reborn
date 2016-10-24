// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/multi_head_manager.h"

#include <QDebug>

#include "ui/widgets/wallpaper_item.h"
#include "ui/utils/xrandr_wrapper.h"

namespace installer {

MultiHeadManager::MultiHeadManager(QObject* parent)
    : QObject(parent),
      wallpaper_items_() {
  this->setObjectName(QStringLiteral("wallpaper_manager"));
}

void MultiHeadManager::updateWallpaper() {
  // Clear old wallpaper items
  for (WallpaperItem* item : wallpaper_items_) {
    delete item;
    item = nullptr;
  }
  wallpaper_items_.clear();

  XRandR xrandr;
  QString out;
  RunXRandR(out);

  if (ParseXRandR(out, xrandr)) {
    for (const Output& output : xrandr.outputs) {
      if (output.is_connected) {
        const QRect geometry(output.x, output.y, output.width, output.height);
        WallpaperItem* item = new WallpaperItem(geometry);
        wallpaper_items_.append(item);
        item->show();
      }
    }
  } else {
    qCritical() << "updateWallpaper() parse xrandr failed!";
  }

  for (const Output& output : xrandr.outputs) {
    if (output.is_connected && output.is_primary) {
      const QRect geometry(output.x, output.y, output.width, output.height);
      emit this->primaryScreenChanged(geometry);
      break;
    }
  }
}

void MultiHeadManager::switchXRandRMode() {
  SwitchModeWrapper();
  this->updateWallpaper();
}

}  // namespace installer