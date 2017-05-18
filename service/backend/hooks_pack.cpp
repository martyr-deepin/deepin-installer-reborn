// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hooks_pack.h"

#include <QDebug>
#include <QDir>

#include "base/command.h"
#include "base/file_util.h"
#include "service/settings_manager.h"

namespace installer {

namespace {

const char kBeforeChrootDir[] = "before_chroot";
const char kInChrootDir[] = "in_chroot";
const char kAfterChrootDir[] = "after_chroot";

const char kTargetHooksDir[] = "/tmp/installer";
const char kChrootTargetHooksDir[] = "/target/tmp/installer";

bool AddExecutable(const QString& path, bool recursive) {
  if (recursive) {
    return SpawnCmd("/bin/chmod", {"-R", "a+x", path});
  } else {
    return SpawnCmd("/bin/chmod", {"a+x", path});
  }
}

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
    hooks.append(builtin_dir.absoluteFilePath(file));
  }

  return hooks;
}

bool RemoveFolderRecursively(const QString& path) {
  return SpawnCmd("/bin/rm", {"-r", "-f", path});
}

}  // namespace

void HooksPack::init(HookType type, int progress_begin, int progress_end,
                     HooksPack* next) {
  this->type = type;
  this->progress_begin = progress_begin;
  this->progress_end = progress_end;
  this->next = next;
  this->hooks = ListHooks(type);
  this->current_hook = -1;
}

bool CopyHooks() {
  // First, remove old folder.
  if (!RemoveFolderRecursively(kTargetHooksDir)) {
    qCritical() << "Failed to remove hooks folder:" << kTargetHooksDir;
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

  // Add executable permissions
  if (!AddExecutable(kTargetHooksDir, true)) {
    qCritical() << "Failed to add executable permissions to hooks";
    return false;
  }

  return true;
}

bool ChrootCopyHooks() {
  // Remove old folders.
  if (!RemoveFolderRecursively(kChrootTargetHooksDir)) {
    qCritical() << "Failed to remove hooks folder:"
                << kChrootTargetHooksDir;
    return false;
  }

  if (!CopyFolder(kTargetHooksDir, kChrootTargetHooksDir)) {
    qCritical() << "Failed to copy hooks to:" << kChrootTargetHooksDir;
    return false;
  }

  // Add executable permissions
  if (!AddExecutable(kChrootTargetHooksDir, true)) {
    qCritical() << "Failed to add executable permissions to hooks:"
                << kChrootTargetHooksDir;
    return false;
  }

  return true;
}

}  // namespace installer
