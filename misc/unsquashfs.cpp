// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#define _XOPEN_SOURCE 500  // Required by nftw().
#include <fcntl.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <unistd.h>

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
QString g_progress_file;
QString g_dest_dir;

int CopyFile(const char* fpath, const struct stat* sb,
             int typeflag, struct FTW* ftwbuf) {
  Q_UNUSED(sb);
  Q_UNUSED(typeflag);

  const QString dest_filepath =
      QDir(g_dest_dir).absoluteFilePath(fpath + ftwbuf->base);
  const size_t dest_len = size_t(dest_filepath.length());
  char dest_file[dest_len + 1];
  strncpy(dest_file, dest_filepath.toUtf8().data(), dest_len);
  dest_file[dest_len+1] = '\0';
  qDebug() << "dest filepath:" << dest_filepath;

  int src_fd, dest_fd;
  src_fd = open(fpath, O_RDONLY);
  if (src_fd == -1) {
    qCritical() << "CopyFile() Failed to open src file:" << fpath;
    return 1;
  }

  dest_fd = open(dest_file, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
  if (dest_fd == -1) {
    qCritical() << "CopyFile() Failed to open dest file:" << dest_filepath;
    perror("Open dest file failed:!");
    return 1;
  }

  size_t num_to_read = size_t(sb->st_size);
  bool ok = true;
  while (num_to_read > 0) {
    const ssize_t num_sent = sendfile(dest_fd, src_fd, NULL, num_to_read);
    if (num_sent <= 0) {
      perror("sendfile() err:");
      ok = false;
      break;
    }
    num_to_read -= num_sent;
  }

  close(src_fd);
  close(dest_fd);

  // Update unmask
  // Update xattr

  return ok ? 0 : 1;
}

// Copy files from |mount_point| to |dest_dir|, keeping xattrs.
bool CopyFiles(const QString& src_dir, const QString& dest_dir,
               const QString& progress_file) {
  qDebug() << "CopyFiles():" << src_dir << dest_dir << progress_file;
  if (!installer::CreateDirs(dest_dir)) {
    qCritical() << "CopyFiles() create dest dir failed:" << dest_dir;
    return false;
  }

  g_progress_file = progress_file;
  g_dest_dir = dest_dir;

  const int flag = FTW_DEPTH;
  return (nftw(src_dir.toUtf8().data(), CopyFile,
               kMaxOpenFd, flag) == 0);
}
//
//// Mount filesystem at |src| to |mount_point|
//bool MountFs(const QString& src, const QString& mount_point) {
//  if (!installer::CreateDirs(mount_point)) {
//    qCritical() << "MountFs() Failed to create folder:" << mount_point;
//    return false;
//  }
//  QString output, err;
//  bool ok = installer::SpawnCmd("mount", {src, mount_point}, output, err);
//  if (!ok) {
//    qCritical() << "MountFs() err:" << err;
//  }
//
//  // TODO(xushaohua): Check |mount_point| dir is not empty.
//
//  return ok;
//}

// Umount filesystem from |mount_point|.
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

//  if (!MountFs(src, mount_point)) {
//    qCritical() << "Mount" << src << "to" << mount_point << "failed!";
//    exit(kExitErr);
//  }

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