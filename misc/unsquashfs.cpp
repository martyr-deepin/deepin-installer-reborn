// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#define _XOPEN_SOURCE 500  // Required by nftw().
#include <ftw.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "base/command.h"
#include "base/file_util.h"

namespace {

const char kAppName[] = "deepin-installer-unsqaushfs";
const char kAppDesc[] = "Tool to extract squashfs filesystem";
const char kAppVersion[] = "0.0.1";

// Default folder name of target.
const char kDefaultDest[] = "squashfs-root";

// Absolute folder path to mount filesystem to.
const char kMountPoint[] = "/dev/shm/installer-unsquashfs";

const int kExitOk = 0;
const int kExitErr = 1;

// Maximum number of opended file descriptors.
const int kMaxOpenFd = 36;

// Global references to progress_file and dest_dir.
char* g_progress_file = nullptr;
char* g_dest_dir = nullptr;

int CopyFile(const char* fpath, const struct stat* sb,
             int typeflag, struct FTW* ftwbuf) {
  Q_UNUSED(fpath);
  Q_UNUSED(sb);
  Q_UNUSED(typeflag);
  Q_UNUSED(ftwbuf);
  return 0;
}

bool CopyFiles(const QString& mount_point, const QString& dest_dir,
               const QString& progress_file) {
  if (!installer::CreateDirs(dest_dir)) {
    qCritical() << "CopyFiles() create dest dir failed:" << dest_dir;
    return false;
  }

  {
    const size_t length = size_t(dest_dir.length());
    g_dest_dir = new char[length];
    strncpy(g_dest_dir, dest_dir.toUtf8().data(), length);
  }
  {
    const size_t length = size_t(progress_file.length());
    g_progress_file = new char[length];
    strncpy(g_progress_file, progress_file.toUtf8().data(), length);
  }

  const int flag = FTW_DEPTH;
  const bool ok = (nftw(mount_point.toUtf8().data(), CopyFile,
                        kMaxOpenFd, flag) == 0);

  delete[] g_progress_file;
  g_progress_file = nullptr;
  delete[] g_dest_dir;
  g_dest_dir = nullptr;

  return ok;
}

bool MountFs(const QString& src, const QString& mount_point) {
  if (!installer::CreateDirs(mount_point)) {
    qCritical() << "MountFs() Failed to create folder:" << mount_point;
    return false;
  }
  QString output, err;
  bool ok = installer::SpawnCmd("mount", {src, mount_point}, output, err);
  if (!ok) {
    qCritical() << "MountFs() err:" << err;
  }
  return ok;
}

bool UnMountFs(const QString& mount_point) {
  return installer::SpawnCmd("umount", {mount_point});
}

}  // namespace

int main(int argc, char* argv[]) {
  // Parse args
  // Mount squashfs file
  // Copy files to target
  // Unmount from mount-point

  QCoreApplication app(argc, argv);
  app.setApplicationVersion(kAppVersion);

  QCommandLineParser parser;
  const QCommandLineOption dest_option(
      "dest", "extract to <pathname>, default \"squashfs-root\"",
      "pathname", kDefaultDest);
  parser.addOption(dest_option);
  const QCommandLineOption progress_option(
      "progress","print progress info to \"file\"",
      "file", "");
  parser.addOption(progress_option);
  parser.setApplicationDescription(kAppDesc);
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file", "squashfs filesystem to be extracted");

  if (!parser.parse(app.arguments())) {
    parser.showHelp(kExitErr);
  }

  if (parser.isSet("version") || parser.isSet("help")) {
    // Show help and exit.
    parser.showHelp(kExitOk);
  }

  const QStringList positional_args = parser.positionalArguments();
  if (positional_args.length() != 1) {
    qCritical() << "Too many files to extract, expect one!";
    parser.showHelp(kExitErr);
  }

  const QString src(positional_args.at(0));
  const QFile src_file(src);
  if (!src_file.exists()) {
    qCritical() << "File not found!:" << src;
    parser.showHelp(kExitErr);
  }
  if (src_file.size() == 0) {
    qCritical() << "Filesystem is empty!" << src;
    parser.showHelp(kExitErr);
  }

  const QString mount_point(kMountPoint);

  const QString dest_dir = parser.value(dest_option);
  const QString progress_file = parser.value(progress_option);

  if (!MountFs(src, mount_point)) {
    qCritical() << "Mount" << src << "to" << mount_point << "failed!";
    exit(kExitErr);
  }

  const bool ok = CopyFiles(mount_point, dest_dir, progress_file);
  if (!ok) {
    qCritical() << "Copy files failed!";
  }

  if (!UnMountFs(mount_point)) {
    qCritical() << "UnMount failed:" << mount_point;
    exit(kExitErr);
  }

  exit(ok ? kExitOk : kExitErr);
}