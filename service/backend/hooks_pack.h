// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_BACKEND_HOOKS_PACK_H
#define INSTALLER_SERVICE_BACKEND_HOOKS_PACK_H

#include <QObject>
#include <QStringList>

namespace installer {

enum class HookType {
  BeforeChroot,
  InChroot,
  AfterChroot,
};

struct HooksPack {
  void init(HookType type, int progress_begin, int progress_end,
            HooksPack* next);

  HookType type;
  QStringList hooks;
  int current_hook;
  int progress_begin;
  int progress_end;
  HooksPack* next = nullptr;
};

// Copy hooks from system and oem folder to /tmp/installer-reborn
bool CopyHooks();

// Copy hooks from /tmp/installer-reborn to /target/tmp/installer-reborn
bool ChrootCopyHooks();

}  // namespace installer

#endif  // INSTALLER_SERVICE_BACKEND_HOOKS_PACK_H
