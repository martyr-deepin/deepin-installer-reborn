// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_WALLPAPER_MANAGER_H
#define INSTALLER_WALLPAPER_MANAGER_H

#include <QObject>
#include <QList>
class QDesktopWidget;

namespace installer {

class WallpaperItem;

// Manage wallpapers of multi-head system.
class WallpaperManager : public QObject {
  Q_OBJECT

 public:
  explicit WallpaperManager(QObject* parent = nullptr);

 public slots:
  // Update wallpaper items background current screens information.
  void updateWallpaper();

 private:
  void initConnections();

  QList<WallpaperItem*> wallpaper_items_;
  QDesktopWidget* desktop_widget_ = nullptr;

 private slots:
  void onPrimaryScreenChanged();
  void onScreenCountChanged();
};

}  // namespace installer

#endif  // INSTALLER_WALLPAPER_MANAGER_H
