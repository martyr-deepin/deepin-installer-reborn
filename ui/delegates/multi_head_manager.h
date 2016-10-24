// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_WALLPAPER_MANAGER_H
#define INSTALLER_WALLPAPER_MANAGER_H

#include <QObject>
#include <QList>

namespace installer {

class WallpaperItem;

// Manage wallpapers of multi-head system.
class MultiHeadManager : public QObject {
  Q_OBJECT

 public:
  explicit MultiHeadManager(QObject* parent = nullptr);

 public slots:
  // Update wallpaper items background current screens information.
  void updateWallpaper();

  // Switch display mode.
  void switchXRandRMode();

 signals:
  // Emitted when primary screen changed to |geometry|.
  void primaryScreenChanged(const QRect& geometry);

 private:
  QList<WallpaperItem*> wallpaper_items_;
};

}  // namespace installer

#endif  // INSTALLER_WALLPAPER_MANAGER_H
