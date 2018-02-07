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

#include "service/power_manager.h"

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

const char kSystemCtlCmd[] = "systemctl";
const char kSysReqTriggerFile[] = "/proc/sysrq-trigger";

}  // namespace

bool RebootSystem() {
  return SpawnCmd(kSystemCtlCmd, {"reboot"});
}

bool RebootSystemWithMagicKey() {
  return WriteTextFile(kSysReqTriggerFile, "b");
}

bool ShutdownSystem() {
  return SpawnCmd(kSystemCtlCmd, {"poweroff"});
}

bool ShutdownSystemWithMagicKey() {
  return WriteTextFile(kSysReqTriggerFile, "o");
}

}  // namespace installer