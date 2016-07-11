// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/file_util.h"

#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>

namespace base {

QDir ConcateDir(const QDir& parent_dir, const QString& folder_name) {
  if (! parent_dir.exists(folder_name)) {
    // TODO(xushaohua): Handles permission error.
    parent_dir.mkpath(folder_name);
  }
  return QDir(parent_dir.filePath(folder_name));
}

bool CreateParentDirs(const QString& filepath) {
  return QFileInfo(filepath).absoluteDir().mkpath(".");
}

QString GetFileBasename(const QString& filepath) {
  const int slash_index = filepath.lastIndexOf(QDir::separator());
  QString tmp_filepath;
  if (slash_index > -1) {
    tmp_filepath = filepath.mid(slash_index + 1);
  } else {
    tmp_filepath = filepath;
  }

  const int dot_index = tmp_filepath.lastIndexOf(QChar('.'));
  if (dot_index > 0) {
    return tmp_filepath.left(dot_index);
  } else {
    return tmp_filepath;
  }
}

QString GetFileExtname(const QString& filepath) {
  const int dot_index = filepath.lastIndexOf(QChar('.'));
  if (dot_index > 0) {
    // Ignores hidden file.
    return filepath.mid(dot_index + 1).toLower();
  }

  return "";
}

qint64 GetFileSize(const QString& filepath) {
  QFileInfo info(filepath);
  if (info.exists()) {
    return info.size();
  } else {
    return 0;
  }
}

QString ReadTextFileContent(const QString& path) {
  QFile file(path);
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qWarning() << "[base]::ReadTextFileContent() failed to open" << path;
      return "";
    }
    QTextStream text_stream(&file);
    QString str = text_stream.readAll();
    file.close();
    return str;
  } else {
    qWarning() << "[base]::ReadFileContent() file not found: " << path;
    return "";
  }
}

QString ReadGBKTextFileContent(const QString& path) {
  QFile file(path);
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly)) {
      qWarning() << "[file_util].ReadGBKTextFileContent() failed to open"
                 << path;
      return "";
    }
    const QByteArray file_data = file.readAll();
    QTextCodec* codec = QTextCodec::codecForName("GB18030");
    file.close();
    return codec->toUnicode(file_data);
  } else {
    qWarning() << "[file_util].ReadGBKTextFileContent() file not found:"
    << path;
    return "";
  }
}

bool ReadTextFile(const QString& path, QString& content) {
  QFile file(path);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text_stream(&file);
    content = text_stream.readAll();
    file.close();
    return true;
  } else {
    qWarning() << "[base]::ReadTextFile() failed!" << ", path: " << path;
    return false;
  }
}

bool WriteTextFile(const QString& path, const QString& content) {
  QFile file(path);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream text_stream(&file);
    text_stream << content;
    text_stream.flush();
    file.close();
    return true;
  }else {
    qCritical() << "[base]::WriteTestFile() failed!" << ", path:" << path;
    return false;
  }
}

}  // namespace base