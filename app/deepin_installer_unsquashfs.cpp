/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Extract squash filesystem. Works with low memory machine.
//  * First mount squashfs to system
//  * Then copy each file in that folder to target, including file permissions.
// If extraction progress is required, use --progress option.
// Known issues:
//  * Selected squashfs file can be mounted to one mount-point each time.
//    Or else `mount` command raise device-busy error.

#define _XOPEN_SOURCE 500  // Required by nftw().
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/xattr.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QtMath>

#include "base/command.h"
#include "base/consts.h"
#include "base/file_util.h"

#define S_IMODE 07777

// TODO(xushaohua): Added --debug option.
// TODO(xushaohua): Added --force option.
// TODO(xushaohua): Unmount squashfs file on error.

namespace {

const char kAppName[] = "deepin-installer-unsquashfs";
const char kAppDesc[] = "Tool to extract squashfs filesystem";
const char kAppVersion[] = "0.0.1";

// Default folder name of target.
const char kDefaultDest[] = "squashfs-root";

// Absolute folder path to mount filesystem to.
const char kMountPointTmp[] = "/dev/shm/installer-unsquashfs-%1";

const int kExitOk = 0;
const int kExitErr = 1;

// Maximum number of opened file descriptors.
// See /proc/self/limits for more information.
const int kMaxOpenFd = 512;

// File descriptor of progress file.
FILE* g_progress_fd = nullptr;

// Global references to src_dir and dest_dir.
QString g_src_dir;
QString g_dest_dir;

// Total number of files in squashfs filesystem.
int g_total_files = 0;
// Number of files has been copied.
int g_current_files = 0;

// Use sendfile() system call or not.
bool g_use_sendfile = true;

// Write progress value to file.
void WriteProgress(int progress) {
  if (g_progress_fd) {
    fseek(g_progress_fd, 0, SEEK_SET);
    fprintf(g_progress_fd, "%d", progress);
  } else {
    fprintf(stdout, "\r%d", progress);
  }
}

// Copy regular file with sendfile() system call, from |src_file| to
// |dest_file|. Size of |src_file| is |file_size|.
bool SendFile(const char* src_file, const char* dest_file, ssize_t file_size) {
  int src_fd, dest_fd;
  src_fd = open(src_file, O_RDONLY);
  if (src_fd == -1) {
    fprintf(stderr, "SendFile() Failed to open src file: %s\n", src_file);
    perror("Open src file failed!");
    return false;
  }

  // TODO(xushaohua): handles umask
  dest_fd = open(dest_file, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
  if (dest_fd == -1) {
    fprintf(stderr, "SendFile() Failed to open dest file: %s\n", dest_file);
    perror("Open dest file failed!");
    return false;
  }

  bool ok = true;
  if (g_use_sendfile) {
    size_t num_to_read = size_t(file_size);
    while (num_to_read > 0) {
      const ssize_t num_sent = sendfile(dest_fd, src_fd, nullptr, num_to_read);
      if (num_sent <= 0) {
        fprintf(stderr, "sendfile() error: %s\nSkip %s\n",
                strerror(errno), src_file);
        // NOTE(xushaohua): Skip sendfile() error.
        // xz uncompress error, Input/output error.
        // squashfs file might have some defects.
//      ok = false;
        ok = true;
        break;
      }
      num_to_read -= num_sent;
    }
  } else {
    const size_t kBufSize = 32 * 1024;  // 32k
    char buf[kBufSize];
    ssize_t num_read;
    while ((num_read = read(src_fd, buf, kBufSize)) > 0) {
      // TODO(xushaohua): write() may write less buf.
      if (write(dest_fd, buf, (size_t)num_read) != num_read) {
        ok = false;
        break;
      }
    }
    if (num_read < 0) {
      ok = false;
    }
  }

  close(src_fd);
  close(dest_fd);

  return ok;
}

bool CopySymLink(const char* src_file, const char* link_path) {
  char buf[PATH_MAX];
  ssize_t link_len = readlink(src_file, buf, PATH_MAX);
  if (link_len <= 0) {
    fprintf(stderr, "CopySymLink() readlink() failed: %s\n", src_file);
    perror("readlink() error");
    return false;
  }

  char target[link_len + 1];
  strncpy(target, buf, (size_t)link_len);
  target[link_len] = '\0';
  if (symlink(target, link_path) != 0) {
    fprintf(stderr, "CopySymLink() symlink() failed, %s (%s -> %s)\n",
            strerror(errno), link_path, target);
    // Ignores EEXIST.
    return (errno == EEXIST);
  } else {
    return true;
  }
}

// Update xattr (access control lists and file capabilities)
bool CopyXAttr(const char* src_file, const char* dest_file) {
  bool ok = true;
  // size of extended attribute list, 64k.
  char list[XATTR_LIST_MAX];
  char value[XATTR_NAME_MAX];
  ssize_t xlist_len = llistxattr(src_file, list, XATTR_LIST_MAX);
  if (xlist_len < 0) {
    // Check errno.
    if (errno == ENOTSUP) {
      // Target filesystem does not support extended attributes.
      ok = true;
    } else {
      fprintf(stdout, "CopyXAttr() llistxattr() failed: %s, %s\n", src_file,
              strerror(errno));
      ok = false;
    }
  } else {
    ssize_t value_len;
    for (int ns = 0; ns < xlist_len; ns += strlen(&list[ns] + 1)) {
      value_len = lgetxattr(src_file, &list[ns], value, XATTR_NAME_MAX);
      if (value_len == -1) {
        fprintf(stdout, "CopyXAttr() could not get value: %s\n", src_file);
        break;
      } else {
        if (lsetxattr(dest_file, &list[ns], value, size_t(value_len), 0) != 0) {
          fprintf(stdout, "CopyXAttr() setxattr() failed: %s, %s, %s, %s\n",
                  dest_file, &list[ns], value, strerror(errno));
          ok = false;
          break;
        }
      }
    }
  }

  return ok;
}

// Tree walk handler. Copy one item from |fpath|.
int CopyItem(const char* fpath, const struct stat* sb,
             int typeflag, struct FTW* ftwbuf) {
  Q_UNUSED(sb);
  Q_UNUSED(typeflag);
  Q_UNUSED(ftwbuf);

  struct stat st;
  if (lstat(fpath, &st) != 0) {
    fprintf(stderr, "CopyItem() call lstat() failed: %s\n", fpath);
    perror("lstat()");
    return 1;
  }

  QString relative_path(fpath);
  relative_path.remove(g_src_dir);
  if (relative_path.startsWith('/')) {
    relative_path = relative_path.mid(1);
  }
  const QString dest_filepath =
      QDir(g_dest_dir).absoluteFilePath(relative_path);

  // Create parent dirs.
  installer::CreateParentDirs(dest_filepath);

  const std::string std_dest_filepath(dest_filepath.toStdString());
  const char* dest_file = std_dest_filepath.c_str();

  // Get file mode.
  const mode_t mode = st.st_mode & S_IMODE;
  bool ok = true;

  // Remove dest_file if it exists.
  struct stat dest_stat;
  if (stat(dest_file, &dest_stat) == 0) {
    if (!S_ISDIR(dest_stat.st_mode)) {
      unlink(dest_file);
    }
  }

  if (S_ISLNK(st.st_mode)) {
    // Symbolic link
    ok = CopySymLink(fpath, dest_file);
  } else if (S_ISREG(sb->st_mode)) {
    // Regular file
    ok = SendFile(fpath, dest_file, st.st_size);
  } else if (S_ISDIR(st.st_mode)) {
    // Directory
    ok = installer::CreateDirs(dest_filepath);
  } else if (S_ISCHR(st.st_mode)) {
    // Character device
    ok = (mknod(dest_file, mode | S_IFCHR, st.st_dev) == 0);
  } else if (S_ISBLK(st.st_mode)) {
    // For block device.
    ok = (mknod(dest_file, mode | S_IFBLK, st.st_dev) == 0);
  } else if (S_ISFIFO(st.st_mode)) {
    // FIFO
    ok = (mknod(dest_file, mode | S_IFIFO, st.st_dev) == 0);
  } else if (S_ISSOCK(st.st_mode)) {
    // Socket
    ok = (mknod(dest_file, mode | S_IFSOCK, st.st_dev) == 0);
  } else {
    fprintf(stderr, "CopyItem() Unknown file mode: %d\n", st.st_mode);
  }

  if (!ok) {
    fprintf(stderr, "Failed to copy item: %s\n", dest_file);
    // Ignore copy file error.
    // Return if error occurs
//    return 1;
  }

  // Update ownership first, or chmod() might ignore SUID/SGID or sticky flag.
  if (lchown(dest_file, st.st_uid, st.st_gid) != 0) {
    fprintf(stderr, "CopyItem() lchown() failed: %s, %d, %d\n",
            dest_file, st.st_uid, st.st_gid);
    perror("lchown()");
    // Ignores copy file error.
//    ok = false;
  }
  // Update permissions.
  if (!S_ISLNK(st.st_mode)) {
    if (chmod(dest_file, mode) != 0) {
      fprintf(stderr, "CopyItem() chmod failed: %s, %ul\n", dest_file, mode);
      perror("chmod()");
      // Ignores chmod error.
//      ok = false;
    }
  }

  if (!CopyXAttr(fpath, dest_file)) {
    // NOTE(xushaohua): Do not exit when failed to copy file capacities.
    // This may be happen in Alpha based computer.
    fprintf(stderr, "CopyXAttr() failed: %s\n", fpath);
//    ok = false;
  }

  g_current_files ++;
  const int progress = qFloor(g_current_files * 100.0 / g_total_files);
  WriteProgress(progress);

  return ok ? 0 : 1;
}

int CountItem(const char* fpath, const struct stat* sb,
              int typeflag, struct FTW* ftwbuf) {
  Q_UNUSED(fpath);
  Q_UNUSED(sb);
  Q_UNUSED(typeflag);
  Q_UNUSED(ftwbuf);
  g_total_files ++;
  return 0;
}

// Copy files from |mount_point| to |dest_dir|, keeping xattrs.
bool CopyFiles(const QString& src_dir, const QString& dest_dir,
               const QString& progress_file) {
  if (!installer::CreateDirs(dest_dir)) {
    fprintf(stderr, "CopyFiles() failed to create dest dir: %s\n",
            dest_dir.toLocal8Bit().constData());
    return false;
  }

  // Save current umask.
  const mode_t old_mask = umask(0);

  if (!progress_file.isEmpty()) {
    // Set progress file descriptor.
    g_progress_fd = fopen(progress_file.toStdString().c_str(), "w");
    if (g_progress_fd == nullptr) {
      perror("fopen() Failed to open progress file");
    }
  }

  g_src_dir = src_dir;
  g_dest_dir = dest_dir;

  // Count file numbers.
  bool ok = (nftw(src_dir.toUtf8().data(),
                  CountItem, kMaxOpenFd, FTW_PHYS) == 0);
  if (!ok || (g_total_files == 0)) {
    fprintf(stderr, "CopyFiles() Failed to count file number!\n");
  } else {
    ok = (nftw(src_dir.toUtf8().data(), CopyItem, kMaxOpenFd, FTW_PHYS) == 0);
  }

  // Reset umask.
  umask(old_mask);

  if (ok) {
    WriteProgress(100);
  }

  if (g_progress_fd) {
    fclose(g_progress_fd);
  }

  return ok;
}

// Mount filesystem at |src| to |mount_point|
bool MountFs(const QString& src, const QString& mount_point) {
  if (!installer::CreateDirs(mount_point)) {
    fprintf(stderr, "MountFs() failed to create folder: %s\n",
            mount_point.toLocal8Bit().constData());
    return false;
  }
  QString output, err;
  const bool ok = installer::SpawnCmd("mount", {src, mount_point}, output, err);
  if (!ok) {
    fprintf(stderr, "MountFs() err: %s\n", err.toLocal8Bit().constData());
  }

  // TODO(xushaohua): Check |mount_point| dir is not empty.

  return ok;
}

// Umount filesystem from |mount_point|.
bool UnMountFs(const QString& mount_point) {
  const bool ok = installer::SpawnCmd("umount", {"-R", mount_point});
  if (!ok) {
    fprintf(stderr, "Umount %s failed!", mount_point.toLocal8Bit().constData());
  }
  return ok;
}

}  // namespace

int main(int argc, char* argv[]) {
  // Parse args
  // Mount squashfs file
  // Copy files to target
  // Unmount from mount-point

  // NOTE(xushaohua): "LANG" might not set in some live environment.
  // Set locale to en_US.UTF-8, or else, mounted squashfs will display invalid
  // character code.
  qputenv("LC_ALL", installer::kDefaultLang);
  qputenv("LANG", installer::kDefaultLang);
  (void) setlocale(LC_ALL, installer::kDefaultLang);

  QCoreApplication app(argc, argv);
  app.setApplicationName(kAppName);
  app.setApplicationVersion(kAppVersion);

  QCommandLineParser parser;
  const QCommandLineOption dest_option(
      "dest", "extract to <pathname>, default \"squashfs-root\"",
      "pathname", kDefaultDest);
  parser.addOption(dest_option);
  const QCommandLineOption progress_option(
      "progress","print progress info to <file>",
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
    fprintf(stderr, "Too many files to extract, expect one!\n");
    parser.showHelp(kExitErr);
  }

  const QString src(positional_args.at(0));
  const QFile src_file(src);
  if (!src_file.exists()) {
    fprintf(stderr, "File not found: %s\n", src.toLocal8Bit().constData());
    parser.showHelp(kExitErr);
  }
  if (src_file.size() == 0) {
    fprintf(stderr, "Filesystem is empty: %s\n", src.toLocal8Bit().constData());
    parser.showHelp(kExitErr);
  }

  struct utsname uname_buf;
  if (uname(&uname_buf) == 0) {
    // Do not use sendfile() on "sw" platform, as do_sendfile() always crashes!
    g_use_sendfile = (strncmp(uname_buf.machine, "sw", 2) != 0);
  } else {
    g_use_sendfile = false;
  }
  fprintf(stdout, "use_sendfile: %s\n", g_use_sendfile ? "yes" : "no");

  const qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
  const QString mount_point(QString(kMountPointTmp).arg(timestamp));

  const QString dest_dir = parser.value(dest_option);
  const QString progress_file = parser.value(progress_option);

  if (!MountFs(src, mount_point)) {
    fprintf(stderr, "Mount %s to %s failed!\n",
            src.toLocal8Bit().constData(),
            mount_point.toLocal8Bit().constData());
    exit(kExitErr);
  }

  const bool ok = CopyFiles(mount_point, dest_dir, progress_file);
  if (!ok) {
    fprintf(stderr, "Copy files failed!\n");
  }

  // Commit filesystem caches to disk.
//  sync();

  for (int retry = 0; retry < 5; ++retry) {
    if (!UnMountFs(mount_point)) {
      fprintf(stderr, "Unmount %s failed\n",
              mount_point.toLocal8Bit().constData());
      sleep((unsigned int)(retry * 2 + 1));
    } else {
      break;
    }
  }

  exit(ok ? kExitOk : kExitErr);
}
