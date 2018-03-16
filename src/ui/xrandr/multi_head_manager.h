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

#ifndef INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H
#define INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H

#include <QObject>
#include <QRect>
#include <QVector>

class QThread;

namespace installer {

class MultiHeadWorker;
class WallpaperItem;

// Manage wallpapers of multi-head system.
class MultiHeadManager : public QObject {
  Q_OBJECT

 public:
  explicit MultiHeadManager(QObject* parent = nullptr);
  ~MultiHeadManager();

 public slots:
  // Switch display mode.
  void switchXRandRMode();

  // Update wallpaper items background current screens information.
  void updateWallpaper();

 signals:
  // Emitted when primary screen changed to |geometry|.
  void primaryScreenChanged(const QRect& geometry);

  // Emit this signal to notify xrandr to switch to mirror mode.
  void switchToMirrorMode();

 private:
  void initConnections();

  QVector<WallpaperItem*> wallpaper_items_;
  QThread* multi_head_thread_ = nullptr;
  MultiHeadWorker* multi_head_worker_ = nullptr;

  // Number of monitors defined in last xrandr output.
  // This values changes when a new monitor is connected to or an existing
  // monitor is disconnected from system.
  // If so, switch to mirror mode if possible.
  int last_connected_monitors_;

  // Geometry of primary output used at last.
  QRect last_primary_geometry_;

 private slots:
  // Repaint background when output added, changed or removed.
  void onMonitorsChanged();

  void doSwitchToMirrorMode();
};

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_WALLPAPER_MANAGER_H
