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
      last_connected_monitors_(-1),
      last_primary_geometry_() {
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
  qDebug() << "MultiHeadManager switchXRandRMode()";
  SwitchMode();
}

void MultiHeadManager::updateWallpaper() {
  qDebug() << "MultiHeadManager::updateWallpaper()";

  ConnectedOutputs outputs;
  if (GetConnectedOutputs(outputs) && !outputs.isEmpty()) {
    qDebug() << "connected outputs:" << outputs;
    // Clear old wallpaper items
    for (WallpaperItem* item : wallpaper_items_) {
      item->hide();
      delete item;
      item = nullptr;
    }
    wallpaper_items_.clear();

    int primary_index = -1;
    for (int i = 0; i < outputs.length(); ++i) {
      const ConnectedOutput& output = outputs.at(i);
      const QRect geometry(output.x, output.y, output.width, output.height);
      qDebug() << "wallpaper item:" << geometry;
      WallpaperItem* item = new WallpaperItem(geometry);
      wallpaper_items_.append(item);

      item->show();
      // Put wallpaper item behind any sibling widgets.
      item->lower();

      if (output.primary) {
        primary_index = i;
      }
    }

    // Notify main window to change geometry
    if (primary_index == -1) {
      primary_index = 0;
    }
    const ConnectedOutput& primary_output = outputs.at(primary_index);
    const QRect primary_geometry(primary_output.x, primary_output.y,
                                 primary_output.width, primary_output.height);
    if (primary_geometry != last_primary_geometry_) {
      last_primary_geometry_ = primary_geometry;
      emit this->primaryScreenChanged(primary_geometry);
    }

    // Number of monitors changed.
    if (outputs.length() != last_connected_monitors_) {
      qDebug() << "emit switchToMirrorMode() signal";
      last_connected_monitors_ = outputs.length();
      emit this->switchToMirrorMode();
    }
  } else {
    qCritical() << "Failed to get connected outputs";
  }
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
  if (!SwitchToMirrorMode()) {
    qCritical() << "Failed to switch to mirror mode!";
  }
}

}  // namespace installer