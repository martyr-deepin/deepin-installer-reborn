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
const char kChrootCheckModeHooksDir[] = "/target/deepin-installer";

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

  if (GetSettingsBool("system_check_mode")) {
      // copy check mode hooks files
      if (!CopyFolder(BUILTIN_CHECK_HOOKS_DIR, kChrootCheckModeHooksDir)) {
          qCritical() << "Copy check mode hooks folder failed";
          return false;
      }

      // copy check mode hooks files
      const QString check_mode_oem_dir = GetOemCheckHooksDir();
      if (QDir(check_mode_oem_dir).exists()) {
          if (!CopyFolder(check_mode_oem_dir, kChrootCheckModeHooksDir)) {
              qCritical() << "Copy oem check mode hooks folder failed";
              return false;
          }
      }
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
