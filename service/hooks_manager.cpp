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
#include <QHash>
#include <QTimer>

#include "base/command.h"
#include "base/file_util.h"
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

const int kBeforeChrootEndVal = 60;
const int kInChrootStartVal = 60;
const int kInChrootEndVal = 80;
const int kAfterChrootStartVal = 80;
const int kAfterChrootEndVal = 100;

const char kUnsquashfsBaseProgressFile[] = "/dev/shm/unsquashfs_base_progress";
const char kUnsquashfsLangProgressFile[] = "/dev/shm/unsquashfs_lang_progress";
// Interval to read unsquashfs progress file, 1000ms.
const int kReadUnsquashfsInterval = 1000;

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

// Check whether ISO/overlay/ contains *.module files.
bool IsOverlayModuleExists() {
  QDir overlay_dir(GetOverlayFilesystemDir());
  const QStringList modules = overlay_dir.entryList({"*.module"});
  return !modules.isEmpty();
}

int ReadProgressValue(const QString& file) {
  const QString val = ReadTextFileContent(file);
  if (val.isEmpty()) {
    return -1;
  }
  return val.toInt();
}

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  qDebug() << "RunHook():" << hook;
  return RunScriptFile(hook);
}

}  // namespace

HooksManager::HooksManager(QObject* parent)
    : QObject(parent),
      chroot_fd_(0),
      unsquashfs_timer_(new QTimer(this)),
      unsquashfs_stage_(UnsquashfsStage::UnInitialized),
      overlay_filesystem_exists_(false) {
  this->setObjectName(QStringLiteral("hooks_manager"));

  unsquashfs_timer_->setInterval(kReadUnsquashfsInterval);
  this->initConnections();
}

void HooksManager::initConnections() {
  connect(this, &HooksManager::runHooks,
          this, &HooksManager::handleRunHooks);

  connect(unsquashfs_timer_, &QTimer::timeout,
          this, &HooksManager::handleReadUnsquashfsTimeout);
}

bool HooksManager::bindHooks(HooksManager::HookType hook_type) {
  qDebug() << "bindHooks()";
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

bool HooksManager::unbindHooks() {
  qDebug() << "unbindHooks()";
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

QStringList HooksManager::listHooks(HooksManager::HookType hook_type) {
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

  QDir builtin_dir(QDir(kBuiltinBindDir).absoluteFilePath(folder_name));

  const QStringList filter = { "*.job" };
  const QStringList builtin_files =
      builtin_dir.entryList(filter, QDir::Files | QDir::NoDotAndDotDot);
  foreach(const QString& file, builtin_files) {
    if (MatchArchitecture(file)) {
      hooks.insert(file, builtin_dir.absoluteFilePath(file));
    }
  }

  QDir oem_dir(QDir(kOemBindDir).absoluteFilePath(folder_name));
  if (oem_dir.exists()) {
    const QStringList oem_files =
        oem_dir.entryList(filter, QDir::Files | QDir::NoDotAndDotDot);
    foreach(const QString& file, oem_files) {
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

  for (int i = 0; i < hooks.length(); ++i) {
    const QString& hook = hooks.at(i);
    if (!RunHook(hook)) {
      qCritical() << QString(
          "[HooksManager]::runHooksPack() job failed: %1").arg(hook);
      emit this->errorOccurred();
      return false;
    }
    const int progress = progress_begin +
        (progress_end - progress_begin) / hooks.length() * i;
    qDebug() << "processUpdate():" << progress;
    emit this->processUpdate(progress);
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

void HooksManager::monitorProgressFiles() {
  qDebug() << "monitorProgressFiles()";
  // Remove old progress files first.
  QFile::remove(kUnsquashfsLangProgressFile);
  QFile::remove(kUnsquashfsBaseProgressFile);
  overlay_filesystem_exists_ = IsOverlayModuleExists();
  unsquashfs_timer_->start();
}

void HooksManager::handleRunHooks() {
  qDebug() << "handleRunHooks()";
  if (!bindHooks(HookType::BeforeChroot)) {
    qCritical() << "handleRunHooks() failed to call bindHooks()";
    emit this->errorOccurred();
    return;
  }

  // Setup filesystem watch of unsquashfs progress file.
  this->monitorProgressFiles();

  // Run hooks one by one, if some job fails, returns immediately.
  if (!runHooksPack(HookType::BeforeChroot, kBeforeChrootStartVal,
                    kBeforeChrootEndVal)) {
    qCritical() << "handleRunHooks() error occurs in Before_Chroot";
    unbindHooks();
    return;
  }

  if (!bindHooks(HookType::InChroot)) {
    qCritical() << "handleRunHooks() failed to call bindHooks()";
    emit this->errorOccurred();
    return;
  }
  if (!runHooksPack(HookType::InChroot, kInChrootStartVal, kInChrootEndVal)) {
    qCritical() << "handleRunHooks() error occurs in In_Chroot";
    unbindHooks();
    return;
  }

  if (!runHooksPack(HookType::AfterChroot, kAfterChrootStartVal,
                    kAfterChrootEndVal)) {
    qCritical() << "handleRunHooks() error occurs in After_Chroot";
    unbindHooks();
    return;
  }
  unbindHooks();
  emit this->finished();
}

void HooksManager::handleReadUnsquashfsTimeout() {
  switch (unsquashfs_stage_) {
    case UnsquashfsStage::UnInitialized: {
      const int val = ReadProgressValue(kUnsquashfsBaseProgressFile);
      if (val != -1) {
        unsquashfs_stage_ = UnsquashfsStage::ReadBase;
      }
    }
    case UnsquashfsStage::ReadBase: {
      const int val = ReadProgressValue(kUnsquashfsBaseProgressFile);
      int progress;
      if (overlay_filesystem_exists_) {
        progress = kBeforeChrootStartVal +
            (kBeforeChrootEndVal - kBeforeChrootStartVal - 10) * val / 100;
      } else {
        progress = kBeforeChrootStartVal +
            (kBeforeChrootEndVal -kBeforeChrootStartVal) * val / 100;
      }
      emit this->processUpdate(progress);

      if (val >= 99) {
        if (overlay_filesystem_exists_) {
          unsquashfs_stage_ = UnsquashfsStage::ReadLang;
        } else {
          // No need to read overlay progress, so stop timer right now.
          unsquashfs_timer_->stop();
        }
      }
    }
    case UnsquashfsStage::ReadLang: {
      const int val = ReadProgressValue(kUnsquashfsBaseProgressFile);
      const int progress = kBeforeChrootEndVal - 10 + 10 * val / 100;
      emit this->processUpdate(progress);

      if (progress >= 99) {
        unsquashfs_timer_->stop();
      }
    }
  }
}

}  // namespace installer