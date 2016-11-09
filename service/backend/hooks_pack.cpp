// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hooks_pack.h"

#include <QDebug>
#include <QDir>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "sysinfo/machine.h"

namespace installer {

namespace {

const char kBeforeChrootDir[] = "before_chroot";
const char kInChrootDir[] = "in_chroot";
const char kAfterChrootDir[] = "after_chroot";

const char kTargetDir[] = "/target";
const char kInstallerHooksDir[] = "/dev/shm/installer-hooks";

bool MatchArchitecture(const QString& name);

// Returns a list of sorted hook scripts with |hook_type|.
QStringList ListHooks(HookType hook_type) {
  qDebug() << "listHooks()";
  // filename => abs-filepath
  QStringList hooks;
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
  const QDir::Filters dir_filter = QDir::Files | QDir::NoDotAndDotDot;
  QDir dir(QDir(kInstallerHooksDir).absoluteFilePath(folder_name));
  for (const QString& file :
      dir.entryList(name_filter, dir_filter, QDir::Name)) {
    if (MatchArchitecture(file)) {
      hooks.append(dir.absoluteFilePath(file));
    }
  }

  qDebug() << "hooks:" << hooks;
  return hooks;
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
  this->current_hook = -1;
}

bool CopyHooks(bool in_chroot) {
  QString dest_dir;
  if (in_chroot) {
    dest_dir = QString("%1%2").arg(kTargetDir).arg(kInstallerHooksDir);
  } else {
    dest_dir = kInstallerHooksDir;
  }
  if (!CreateDirs(dest_dir)) {
    qCritical() << "Failed to create parent dir:" << dest_dir;
    return false;
  }

  // Copy builtin hooks to /tmp/installer-hooks/
  const QString builtin_dir(BUILTIN_HOOKS_DIR);
  if (!CopyFolder(builtin_dir, dest_dir)) {
    qCritical() << "Failed to copy builtin hooks!";
    return false;
  }

  // Copy oem hooks to /tmp/installer-hooks/
  // Job file with same name will be overwritten here.
  const QString oem_dir(GetOemHooksDir());
  if (QDir(oem_dir).exists()) {
    return CopyFolder(oem_dir, dest_dir);
  } else {
    return true;
  }
}

}  // namespace installer