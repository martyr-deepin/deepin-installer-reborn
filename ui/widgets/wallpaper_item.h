// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_WALLPAPER_ITEM_H
#define INSTALLER_WALLPAPER_ITEM_H

#include <QLabel>

namespace installer {

// Displays background image on one screen.
class WallpaperItem : public QLabel {
  Q_OBJECT

 public:
  WallpaperItem(const QRect& background_geometry, QWidget* parent = nullptr);

 private:
  void initUI();
  QRect background_geometry_;
};

} // namespace installer

#endif  // INSTALLER_WALLPAPER_ITEM_H
