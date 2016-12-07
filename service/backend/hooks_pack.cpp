// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hooks_pack.h"

#include <QDebug>
#include <QDir>

#include "base/command.h"
#include "base/file_util.h"
#include "service/settings_manager.h"
#include "sysinfo/machine.h"

namespace installer {

namespace {

const char kBeforeChrootDir[] = "before_chroot";
const char kInChrootDir[] = "in_chroot";
const char kAfterChrootDir[] = "after_chroot";

const char kTargetHooksDir[] = "/dev/shm/installer-hooks";

bool MatchArchitecture(const QString& name);

// Returns a list of sorted hook scripts with |hook_type|.
QStringList ListHooks(HookType hook_type) {
  // Absolute file path to hooks.
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
  QDir builtin_dir(QDir(kTargetHooksDir).absoluteFilePath(folder_name));
  for (const QString& file :
      builtin_dir.entryList(name_filter, dir_filter, QDir::Name)) {
    if (MatchArchitecture(file)) {
      hooks.append(builtin_dir.absoluteFilePath(file));
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

bool CopyHooks() {
  // First, remove old folder.
  // TODO(xushaohua): Added RemoveFolder() method in base/file_util.sh
  if (!SpawnCmd("/bin/rm", {"-r", "-f", kTargetHooksDir})) {
    qCritical() << "Failed to remove folder hook folder:" << kTargetHooksDir;
    return false;
  }

  if (!CopyFolder(BUILTIN_HOOKS_DIR, kTargetHooksDir)) {
    qCritical() << "Copy builtin hooks folder failed";
    return false;
  }

  const QString oem_dir = GetOemHooksDir();
  if (QDir(oem_dir).exists()) {
    if (!CopyFolder(oem_dir, kTargetHooksDir)) {
      qCritical() << "Copy oem hooks folder failed";
      return false;
    }
  }

  return true;
}

}  // namespace installer