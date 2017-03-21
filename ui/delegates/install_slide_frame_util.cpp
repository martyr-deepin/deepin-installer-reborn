// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/install_slide_frame_util.h"

#include <QDebug>

#include "base/file_util.h"
#include "service/settings_manager.h"

namespace installer {

namespace {

const char kDefaultSlide[] = "default";

const char kSlideFolder[] = RESOURCES_DIR "/slide";

}  // namespace

QString GetSlideDir(const QString& locale) {
  QDir installer_dir(kSlideFolder);
  Q_ASSERT(installer_dir.exists());
  QDir oem_dir(GetOemDir());

  // Check existence of folders one by one.
  QFileInfo file_info(oem_dir.absoluteFilePath(locale));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  file_info.setFile(installer_dir.absoluteFilePath(locale));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  file_info.setFile(oem_dir.absoluteFilePath(kDefaultSlide));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  return installer_dir.absoluteFilePath(kDefaultSlide);
}

QStringList GetSlideFiles(const QString& locale) {
  QStringList slide_files;

  QDir slide_dir(GetSlideDir(locale));
  Q_ASSERT(slide_dir.exists());

  // List all png files in slide folder.
  QString filepath;
  for (const QString& filename : slide_dir.entryList({"*.png"}, QDir::Files)) {
    filepath = slide_dir.absoluteFilePath(filename);
    if (QFile::exists(filepath)) {
      slide_files.append(filepath);
    }
  }

  return slide_files;
}

}  // namespace installer