// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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