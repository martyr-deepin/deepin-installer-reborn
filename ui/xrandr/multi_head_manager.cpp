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
      multi_head_thread_(new QThread(this)),
      multi_head_worker_(new MultiHeadWorker()),
      connected_monitors_(1) {
  this->setObjectName("wallpaper_manager");

  multi_head_worker_->moveToThread(multi_head_thread_);

  this->initConnections();

  multi_head_thread_->start();
  emit multi_head_worker_->start();
}

MultiHeadManager::~MultiHeadManager() {
  multi_head_worker_->stop();
  multi_head_thread_->quit();
  multi_head_thread_->wait(3);
}

void MultiHeadManager::switchXRandRMode() {
  SwitchMode();
}

void MultiHeadManager::updateWallpaper() {
  qDebug() << "MultiHeadManager::updateWallpaper()";
  // Clear old wallpaper items
  for (WallpaperItem* item : wallpaper_items_) {
    delete item;
    item = nullptr;
  }
  wallpaper_items_.clear();

//  if () {
//    int connected_monitors = 0;
//    for (const Output& output : xrandr.outputs) {
//      if (output.is_connected) {
//        connected_monitors ++;
//        const QRect geometry(output.x, output.y, output.width, output.height);
//        WallpaperItem* item = new WallpaperItem(geometry);
//        wallpaper_items_.append(item);
//        item->show();
//
//        // Notify main window to change geometry
//        if (output.is_primary) {
//          emit this->primaryScreenChanged(geometry);
//        }
//      }
//    }

//    if (connected_monitors != connected_monitors_) {
//      connected_monitors_ = connected_monitors;
//      emit this->switchToMirrorMode();
//    }
//  } else {
//    qCritical() << "updateWallpaper() parse xrandr failed!";
//  }
}

void MultiHeadManager::initConnections() {
  connect(multi_head_worker_, &MultiHeadWorker::monitorsChanged,
          this, &MultiHeadManager::onMonitorsChanged);
  connect(this, &MultiHeadManager::switchToMirrorMode,
          this, &MultiHeadManager::doSwitchToMirrorMode);
}

void MultiHeadManager::onMonitorsChanged() {
  this->updateWallpaper();
}

void MultiHeadManager::doSwitchToMirrorMode() {
  qDebug() << "Switch to mirror mode";
  SwitchToMirrorMode();
}

}  // namespace installer