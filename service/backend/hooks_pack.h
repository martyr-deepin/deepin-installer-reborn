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

// Copy hooks from system and oem folder to /tmp/installer
bool CopyHooks();

// Copy hooks from /tmp/installer to /target/tmp/installer
bool ChrootCopyHooks();

}  // namespace installer

#endif  // INSTALLER_SERVICE_BACKEND_HOOKS_PACK_H
