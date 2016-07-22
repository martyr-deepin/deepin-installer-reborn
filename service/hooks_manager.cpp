// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/hooks_manager.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include "service/settings_manager.h"
#include "sysinfo/machine.h"

namespace service {

namespace {

const char kTargetDir[] = "/target";

const char kBuiltinBindDir[] = "/dev/shm/di-builtin";
const char kOemBindDir[] = "/dev/shm/di-oem";

bool MatchArchitecture(const QString& name) {
  sysinfo::MachineArch arch = sysinfo::GetMachineArch();
  const QString lower_name = name.toLower();
  if (arch == sysinfo::MachineArch::Unknown) {
    qWarning() << "MatchArchitecture() unknown architecture";
  }

  if (lower_name.lastIndexOf("alpha.job") > 0 &&
      arch != sysinfo::MachineArch::Alpha) {
    return false;
  }
  if (lower_name.lastIndexOf("alpha64.job") > 0 &&
    arch != sysinfo::MachineArch::Alpha64) {
    return false;
  }
  if (lower_name.lastIndexOf("arm.job") > 0 &&
      arch != sysinfo::MachineArch::ARM) {
    return false;
  }
  if (lower_name.lastIndexOf("arm64.job") > 0 &&
      arch != sysinfo::MachineArch::ARM64) {
    return false;
  }
  if (lower_name.lastIndexOf("mips.job") > 0 &&
      arch != sysinfo::MachineArch::MIPS) {
    return false;
  }
  if (lower_name.lastIndexOf("x86.job") > 0 &&
      arch != sysinfo::MachineArch::X86_64) {
    return false;
  }

  return true;
}

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  qDebug() << "RunHook():" << hook;
  // Change working directory.
  if (!QDir::setCurrent(QFileInfo(hook).absolutePath())) {
    qCritical() << "RunHooks() failed to change working directory:"
                << hook;
    return false;
  }
  const QStringList args = { hook };
  return (QProcess::execute(hook, args) == 0);
}

}  // namespace

HooksManager::HooksManager(QObject* parent)
    : QObject(parent),
      chroot_fd_(0) {
  this->setObjectName(QStringLiteral("hooks_manager"));

  connect(this, &HooksManager::runHooks,
          this, &HooksManager::handleRunHooks);
}

bool HooksManager::bindHooks(HooksManager::HookType hook_type) {
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
    return false;
  }
  return true;
}

bool HooksManager::unbindHooks() {
  qDebug() << "unbindHooks()";
  if (umount(kBuiltinBindDir) != 0) {
    qCritical() << "unbindHooks() failed to unmount builtin bind folder";
    return false;
  }

  const QString chroot_builtin_dir =
      QString("%1%2").arg(kTargetDir).arg(kBuiltinBindDir);
  if (umount(chroot_builtin_dir.toStdString().c_str()) != 0) {
    qCritical() << "unbindHooks() failed to unmount target builtin bind folder";
    return false;
  }

  if (QDir(kOemBindDir).exists() && umount(kOemBindDir) != 0) {
    qCritical() << "unbindHooks() failed to unmount oem bind folder";
    return false;
  }

  const QString chroot_oem_dir =
      QString("%1%2").arg(kTargetDir).arg(kOemBindDir);
  if (umount(chroot_oem_dir.toStdString().c_str()) != 0) {
    qCritical() << "unbindHooks() failed to unmount target oem bind folder";
    return false;
  }
  return true;
}

QStringList HooksManager::listHooks(HooksManager::HookType hook_type) {
  QStringList hooks;
  QString folder_name;
  switch (hook_type) {
    case HookType::BeforeChroot: {
      folder_name = "before_chroot";
      break;
    }

    case HookType::InChroot: {
      folder_name = "in_chroot";
      break;
    }

    case HookType::AfterChroot: {
      folder_name = "after_chroot";
      break;
    }
  }
  QDir builtin_dir(QDir(kBuiltinBindDir).absoluteFilePath(folder_name));

  const QStringList filter = { "*.job" };
  const QStringList builtin_files =
      builtin_dir.entryList(filter, QDir::Files | QDir::NoDotAndDotDot);
  foreach(const QString& file, builtin_files) {
    if (MatchArchitecture(file)) {
      hooks.append(builtin_dir.absoluteFilePath(file));
    }
  }

  QDir oem_dir(QDir(kOemBindDir).absoluteFilePath(folder_name));
  if (oem_dir.exists()) {
    const QStringList oem_files =
        oem_dir.entryList(filter, QDir::Files | QDir::NoDotAndDotDot);
    foreach(const QString& file, oem_files) {
      if (MatchArchitecture(file)) {
        hooks.append(oem_dir.absoluteFilePath(file));
      }
    }
  }

  // Sort hooks
  std::sort(hooks.begin(), hooks.end());
  qDebug() << "hooks:" << hooks;
  return hooks;
}

bool HooksManager::runHooksPack(HooksManager::HookType hook_type,
                                int progress_begin, int progress_end) {
  qDebug() << "[HooksManager]::runHooksPack():" << progress_begin
           << progress_end;
  const QStringList hooks = listHooks(hook_type);

  if (hook_type == HookType::InChroot) {
    if (!this->enterChroot()) {
      emit this->errorOccurred();
      return false;
    }
  }

  foreach (const QString& hook, hooks) {
    if (!RunHook(hook)) {
      qCritical() << QString(
          "[HooksManager]::runHooksPack() job failed: %1").arg(hook);
      emit this->errorOccurred();
      return false;
    }
    // TODO(xushaohua): emit processUpdate() signal.
  }

  if (hook_type == HookType::InChroot) {
    if (!this->leaveChroot()) {
      emit this->errorOccurred();
      return false;
    }
  }
  return true;
}

bool HooksManager::enterChroot() {
  qDebug() << "enterChroot()";
  chroot_fd_ = open("/", O_RDONLY);
  if (chroot_fd_ == -1) {
    qCritical() << "[HooksManager]::enterChroot() failed to '/'!";
    return false;
  }

  if (chdir(kTargetDir) != 0) {
    qCritical() << QString(
        "[HooksManager]::enterChroot() failed to call chdir(%s)").arg(
        kTargetDir);
    return false;
  }

  if (chroot(kTargetDir) != 0) {
    qCritical() << "[HooksManager]::enterChroot() failed to call chroot()";
    return false;
  }

  if (chdir("/") != 0) {
    qCritical() << "[HooksManager]::enterChroot() failed to call chdir(/)";
    return false;
  }
  return true;
}

bool HooksManager::leaveChroot() {
  qDebug() << "leaveChroot()";
  if (fchdir(chroot_fd_) != 0) {
    qCritical() << QString(
        "[HooksManager]::leaveChroot() fchdir(%1) failed").arg(chroot_fd_);
    return false;
  }
  // Chroot to host environment.
  if (chroot(".") != 0) {
    qCritical() << "[HooksManager]::leaveChroot() failed to call chroot()";
    return false;
  }
  return true;
}

void HooksManager::handleRunHooks() {
  if (!bindHooks(HookType::BeforeChroot)) {
    qCritical() << "handleRunHooks() failed to call bindHooks()";
    emit this->errorOccurred();
    return;
  }

  // Run hooks one by one, if some job fails, returns immediately.
  if (!runHooksPack(HookType::BeforeChroot, 0, 60)) {
    qCritical() << "handleRunHooks() error occurs in Before_Chroot";
    unbindHooks();
    return;
  }

  if (!bindHooks(HookType::InChroot)) {
    qCritical() << "handleRunHooks() failed to call bindHooks()";
    emit this->errorOccurred();
    return;
  }
  if (!runHooksPack(HookType::InChroot, 60, 80)) {
    qCritical() << "handleRunHooks() error occurs in In_Chroot";
    unbindHooks();
    return;
  }

  if (!runHooksPack(HookType::AfterChroot, 80, 100)) {
    qCritical() << "handleRunHooks() error occurs in After_Chroot";
    unbindHooks();
    return;
  }
  unbindHooks();
  emit this->finished();
}

}  // namespace service