// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_FILE_UTIL_H
#define INSTALLER_BASE_FILE_UTIL_H

#include <QDir>
#include <QString>

namespace installer {

// Create a folder with |folder_name| in |parent_dir| directory and
// returns a QDir object refering to its absolute path.
QDir ConcateDir(const QDir& parent_dir, const QString& folder_name);

// Create parent folders and itself.
bool CreateDirs(const QString& dirpath);

// Create parent folders.
bool CreateParentDires(const QString& filepath);

// Returns final component of |filepath|.
QString GetFileBasename(const QString& filepath);

// Returns lower cased extension name of |filepath|, might be empty.
QString GetFileExtname(const QString& filepath);

// Get size of file. If file not found or has no access, returns 0.
qint64 GetFileSize(const QString& filepath);

// Read contents of file, returns an empty string if failed.
// DEPRECATED: call ReadTextFile() instead.
QString ReadTextFileContent(const QString& path);

// Read text file encoded in GB18030.
QString ReadGBKTextFileContent(const QString& path);

// Read contents of file, returns true if succeeded, or false otherwise.
// |content| holds the content of that file.
bool ReadTextFile(const QString& path, QString& content);

// Write content to file, returns true if succeeded, or false otherwise.
bool WriteTextFile(const QString& path, const QString& content);

}  // namespace installer

#endif  // INSTALLER_BASE_FILE_UTIL_H
