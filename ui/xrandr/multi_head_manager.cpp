// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/xrandr/multi_head_manager.h"

#include <QDebug>
#include <QThread>

#include "ui/widgets/wallpaper_item.h"
#include "ui/xrandr/multi_head_worker.h"
#include "ui/xrandr/xrandr_wrapper.h"

namespace installer {

MultiHeadManager::MultiHeadManager(QObject* parent)
    : QObject(parent),
      wallpaper_items_(),
      worker_thread_(new QThread()) {
  this->setObjectName(QStringLiteral("wallpaper_manager"));

  multi_head_worker_ = new MultiHeadWorker();
  multi_head_worker_->moveToThread(worker_thread_);
  worker_thread_->start();
  emit multi_head_worker_->start();

  this->initConnections();
}

MultiHeadManager::~MultiHeadManager() {
  multi_head_worker_->stop();

  worker_thread_->quit();
  worker_thread_->wait(5);
  delete worker_thread_;
  worker_thread_ = nullptr;
}

void MultiHeadManager::switchXRandRMode() {
  SwitchModeWrapper();
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

        // Notify main window to change geometry
        if (output.is_primary) {
          emit this->primaryScreenChanged(geometry);
        }
      }
    }
  } else {
    qCritical() << "updateWallpaper() parse xrandr failed!";
  }
}

void MultiHeadManager::initConnections() {
  connect(multi_head_worker_, &MultiHeadWorker::screenCountChanged,
          this, &MultiHeadManager::onScreenCountChanged);
}

void MultiHeadManager::onScreenCountChanged() {
  this->updateWallpaper();
}

}  // namespace installer