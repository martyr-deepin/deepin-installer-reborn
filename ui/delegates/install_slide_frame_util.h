// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H
#define INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H

#include <QStringList>

namespace installer {

// Get absolute path to slide folder, based on |locale|.
// |locale| might be empty or like "zh_CN" or "en_US".
QString GetSlideDir(const QString& locale);

// Get slide image file list with specific |locale|.
QStringList GetSlideFiles(const QString& locale);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H