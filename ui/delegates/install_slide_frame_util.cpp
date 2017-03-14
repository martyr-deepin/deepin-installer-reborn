// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/install_slide_frame_util.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "sysinfo/machine.h"

namespace installer {

namespace {

const char kDefaultSlide[] = "default";

const char kSlideFolder[] = RESOURCES_DIR "/slide";

const char kImagesJson[] = "images.json";

// A struct to hold images.json content.
struct ImagesJson {
  QStringList loongson;
  QStringList sw;
  QStringList x86;
};
bool ParseImagesJson(const QString& images_json_file, ImagesJson& images_json) {
  const QString content(ReadFile(images_json_file));
  if (content.isEmpty()) {
    qWarning() << images_json_file << "is empty";
    return false;
  }

  const QJsonObject obj = QJsonDocument::fromJson(content.toUtf8()).object();
  const QJsonArray loongson_arr = obj.value("loongson").toArray();
  images_json.loongson.clear();
  for (const QJsonValue& value : loongson_arr) {
    images_json.loongson.append(value.toString());
  }

  const QJsonArray sw_arr = obj.value("sw").toArray();
  images_json.sw.clear();
  for (const QJsonValue& value : sw_arr) {
    images_json.sw.append(value.toString());
  }

  const QJsonArray x86_arr = obj.value("x86").toArray();
  images_json.x86.clear();
  for (const QJsonValue& value : x86_arr) {
    images_json.x86.append(value.toString());
  }

  return true;
}

}  // namespace

QString GetSlideDir(const QString& language_name) {
  QDir installer_dir(kSlideFolder);
  Q_ASSERT(installer_dir.exists());
  QDir oem_dir(GetOemDir());

  // Check existence of folders one by one.
  QFileInfo file_info(oem_dir.absoluteFilePath(language_name));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  file_info.setFile(installer_dir.absoluteFilePath(language_name));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  file_info.setFile(oem_dir.absoluteFilePath(kDefaultSlide));
  if (file_info.isDir() && file_info.exists()) {
    return file_info.absoluteFilePath();
  }

  return installer_dir.absoluteFilePath(kDefaultSlide);
}

// Get slide image file list with specific |language_name|.
QStringList GetSlideFiles(const QString& language_name) {
  QStringList slide_files;

  QDir slide_dir(GetSlideDir(language_name));
  Q_ASSERT(slide_dir.exists());

  const QString image_json_file = slide_dir.absoluteFilePath(kImagesJson);
  if (QFile::exists(image_json_file)) {
    QStringList file_names;
    ImagesJson images_json;
    // Parse images.json.
    if (ParseImagesJson(image_json_file, images_json)) {
      // Match machine name.
      const MachineArch arch = GetMachineArch();
      switch (arch) {
        case MachineArch::MIPS:  // fall through
        case MachineArch::MIPS64: {
          file_names = images_json.loongson;
          break;
        }
        case MachineArch::SW:  // fall through
        case MachineArch::SW64: {
          file_names = images_json.sw;
          break;
        }
        case MachineArch::X86:  // fall through
        case MachineArch::X86_64: {
          file_names = images_json.x86;
          break;
        }
        default: {
          // Ignore other types.
          break;
        }
      }

      if (!file_names.isEmpty()) {
        QString filepath;
        for (const QString& filename : file_names) {
          filepath = slide_dir.absoluteFilePath(filename);
          // Check file existence.
          if (QFile::exists(filepath)) {
            slide_files.append(filepath);
          }
        }

        if (!slide_files.isEmpty()) {
          return slide_files;
        }
      }
    }
  }

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