// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hooks_pack.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QTimer>

#include "service/settings_manager.h"
#include "sysinfo/machine.h"

namespace installer {

namespace {

const char kTargetDir[] = "/target";

// Target folder hooks/ will be moved to.
const char kBuiltinBindDir[] = "/dev/shm/di-builtin";
const char kOemBindDir[] = "/dev/shm/di-oem";

const char kBeforeChrootDir[] = "before_chroot";
const char kInChrootDir[] = "in_chroot";
const char kAfterChrootDir[] = "after_chroot";

bool MatchArchitecture(const QString& name);

// Returns a list of sorted hook scripts with |hook_type|.
QStringList ListHooks(HookType hook_type) {
  qDebug() << "listHooks()";
  // filename => abs-filepath
  QHash<QString, QString> hooks;
  QString folder_name;
  switch (hook_type) {
    case HookType::BeforeChroot: {
      folder_name = kBeforeChrootDir;
      break;
    }

    case HookType::InChroot: {
      folder_name = kInChrootDir;
      break;
    }

    case HookType::AfterChroot: {
      folder_name = kAfterChrootDir;
      break;
    }
  }

  const QStringList name_filter = { "*.job" };
  // Filter hooks.
  const QDir::Filters dir_filter = QDir::Files | QDir::NoDotAndDotDot |
                                   QDir::Readable | QDir::Executable;
  QDir builtin_dir(QDir(kBuiltinBindDir).absoluteFilePath(folder_name));
  for (const QString& file : builtin_dir.entryList(name_filter, dir_filter)) {
    if (MatchArchitecture(file)) {
      hooks.insert(file, builtin_dir.absoluteFilePath(file));
    }
  }

  QDir oem_dir(QDir(kOemBindDir).absoluteFilePath(folder_name));
  if (oem_dir.exists()) {
    for (const QString& file : oem_dir.entryList(name_filter, dir_filter)) {
      if (MatchArchitecture(file)) {
        // Also override hook file with same name.
        hooks.insert(file, oem_dir.absoluteFilePath(file));
      }
    }
  }

  QStringList filename_list = hooks.keys();
  // Sort files by name.
  std::sort(filename_list.begin(), filename_list.end());
  QStringList result;
  for (const QString& filename : filename_list) {
    result.append(hooks.value(filename));
  }
  qDebug() << "hooks:" << result;
  return result;
}

bool MatchArchitecture(const QString& name) {
  MachineArch arch = GetMachineArch();
  const QString lower_name = name.toLower();
  if (arch == MachineArch::Unknown) {
    qWarning() << "MatchArchitecture() unknown architecture";
    return false;
  }

  if (lower_name.lastIndexOf("alpha.job") > 0 && arch != MachineArch::Alpha) {
    return false;
  }
  if (lower_name.lastIndexOf("alpha64.job") > 0 &&
      arch != MachineArch::Alpha64) {
    return false;
  }
  if (lower_name.lastIndexOf("arm.job") > 0 && arch != MachineArch::ARM) {
    return false;
  }
  if (lower_name.lastIndexOf("arm64.job") > 0 && arch != MachineArch::ARM64) {
    return false;
  }
  if (lower_name.lastIndexOf("mips.job") > 0 && arch != MachineArch::MIPS) {
    return false;
  }
  if (lower_name.lastIndexOf("x86.job") > 0 && arch != MachineArch::X86_64) {
    return false;
  }

  return true;
}

}  // namespace

void HooksPack::init(HookType type, int progress_begin, int progress_end,
                     bool in_chroot, HooksPack* next) {
  this->type = type;
  this->progress_begin = progress_begin;
  this->progress_end = progress_end;
  this->in_chroot = in_chroot;
  this->next = next;
  this->hooks = ListHooks(type);
  this->current_hook = 0;
}

bool BindHooks(HookType hook_type) {
  qDebug() << "BindHooks()";
  QString builtin_dir;
  QString oem_dir;
  if (hook_type == HookType::InChroot) {
    builtin_dir = QString("%1%2").arg(kTargetDir).arg(kBuiltinBindDir);
    oem_dir = QString("%1%2").arg(kTargetDir).arg(kOemBindDir);
  } else {
    builtin_dir = kBuiltinBindDir;
    oem_dir = kOemBindDir;
  }
  if (!QDir(builtin_dir).exists()) {
    if (mkdir(builtin_dir.toStdString().c_str(),
              S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
      qCritical() << "bindHooks() failed to make builtin bind folder"
                  << builtin_dir;
      return false;
    }
  }

  if (mount(BUILTIN_HOOKS_DIR, builtin_dir.toStdString().c_str(),
            NULL, MS_BIND | MS_REC, NULL) != 0) {
    qCritical() << "bindHooks() failed to mount builtin hooks folder:"
                << BUILTIN_HOOKS_DIR;
    qCritical() << strerror(errno);
    return false;
  }

  const QString oem_src_dir(GetOemHooksDir());
  if (!QDir(oem_src_dir).exists()) {
    return true;
  }
  if (mkdir(oem_dir.toStdString().c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
    qCritical() << "bindHooks() failed to make oem bind folder" << oem_dir;
    return false;
  }
  if (mount(oem_src_dir.toStdString().c_str(), oem_dir.toStdString().c_str(),
            NULL, MS_BIND | MS_REC, NULL) != 0) {
    qCritical() << "bindHooks() failed to mount oem hooks folder" << oem_dir;
    qCritical() << strerror(errno);
    return false;
  }
  return true;
}

bool UnbindHooks() {
  qDebug() << "UnbindHooks()";
  if (umount(kBuiltinBindDir) != 0) {
    qCritical() << "unbindHooks() failed to unmount builtin bind folder!\n"
                << strerror(errno);
    return false;
  }

  const QString chroot_builtin_dir =
      QString("%1%2").arg(kTargetDir).arg(kBuiltinBindDir);
  if (umount(chroot_builtin_dir.toStdString().c_str()) != 0) {
    qCritical() << "unbindHooks() failed to unmount target builtin bind folder!"
                << strerror(errno);
    return false;
  }

  if (QDir(kOemBindDir).exists() && umount(kOemBindDir) != 0) {
    qCritical() << "unbindHooks() failed to unmount oem bind folder";
    return false;
  }

  const QString chroot_oem_dir =
      QString("%1%2").arg(kTargetDir).arg(kOemBindDir);
  if (umount(chroot_oem_dir.toStdString().c_str()) != 0) {
    qCritical() << "unbindHooks() failed to unmount target oem bind folder!"
                << strerror(errno);
    return false;
  }
  return true;
}

}  // namespace installer