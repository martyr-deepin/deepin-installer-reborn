// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/hooks_manager.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QTimer>

#include "base/file_util.h"
#include "base/thread_util.h"
#include "service/backend/hooks_pack.h"
#include "service/backend/hook_worker.h"

namespace installer {

namespace {

const int kBeforeChrootEndVal = 60;
const int kInChrootStartVal = kBeforeChrootEndVal;
const int kInChrootEndVal = 85;
const int kAfterChrootStartVal = kInChrootEndVal;
const int kAfterChrootEndVal = 100;

const char kUnsquashfsProgressFile[] = "/dev/shm/unsquashfs_progress";
// Interval to read unsquashfs progress file, 500ms.
const int kReadUnsquashfsInterval = 500;

int ReadProgressValue(const QString& file) {
  if (QFile::exists(file)) {
    const QString val(ReadFile(file));
    if (!val.isEmpty()) {
      return val.toInt();
    }
  }
  return 0;
}

}  // namespace

HooksManager::HooksManager(QObject* parent)
    : QObject(parent),
      hook_worker_(new HookWorker()),
      hook_worker_thread_(new QThread(this)),
      unsquashfs_timer_(new QTimer(this)) {
  this->setObjectName("hooks_manager");

  hook_worker_->moveToThread(hook_worker_thread_);
  this->initConnections();

  hook_worker_thread_->start();
}

HooksManager::~HooksManager() {
  QuitThread(hook_worker_thread_);

  while (hooks_pack_ != nullptr) {
    HooksPack* next_pack = hooks_pack_->next;
    delete hooks_pack_;
    hooks_pack_ = next_pack;
  }
}

void HooksManager::initConnections() {
  connect(this, &HooksManager::runHooks,
          this, &HooksManager::handleRunHooks);
  connect(unsquashfs_timer_, &QTimer::timeout,
          this, &HooksManager::handleReadUnsquashfsTimeout);
  connect(this, &HooksManager::finished,
          this, &HooksManager::onHooksManagerFinished);
  connect(this, &HooksManager::errorOccurred,
          this, &HooksManager::onHooksManagerFinished);
  connect(hook_worker_, &HookWorker::hookFinished,
          this, &HooksManager::onHookFinished);

  // Delete worker object on thread finished.
  connect(hook_worker_thread_, &QThread::finished,
          hook_worker_, &HookWorker::deleteLater);
}

void HooksManager::runNextHook() {
  hooks_pack_->current_hook ++;
  if (hooks_pack_->current_hook >= hooks_pack_->hooks.length()) {
    // Clear environment of current hooks pack.
    if (hooks_pack_->type == HookType::BeforeChroot) {
      unsquashfs_timer_->stop();
    }

    HooksPack* next_hooks_pack = hooks_pack_->next;
    delete hooks_pack_;
    hooks_pack_ = next_hooks_pack;
    if (hooks_pack_ == nullptr) {
      qDebug() << "hooks_pack_ is null, all jobs done!";
      // All hooks pack jobs are finished.
      emit this->finished();
    } else {
      qDebug() << "Run next hooks pack";
      // Run next hooks pack if it is not nullptr
      this->runHooksPack();
    }
  } else {
    // Update progress, except before-chroot.
    if (hooks_pack_->type != HookType::BeforeChroot) {
      const int progress = hooks_pack_->progress_begin +
          int((hooks_pack_->progress_end - hooks_pack_->progress_begin) *
              hooks_pack_->current_hook * 1.0 / hooks_pack_->hooks.length());
      qDebug() << "processUpdate():" << progress;
      emit this->processUpdate(progress);
    }

    // Run next hook in current hooks pack.
    const QString hook = hooks_pack_->hooks.at(hooks_pack_->current_hook);
    qDebug() << "run hook:" << GetFileName(hook);
    emit hook_worker_->runHook(hook);
  }
}

void HooksManager::runHooksPack() {
  Q_ASSERT(hooks_pack_);
  if (!hooks_pack_) {
    qCritical() << "hooks_pack_ is nullptr!";
    emit this->errorOccurred();
    return;
  }

  if (hooks_pack_->type == HookType::BeforeChroot) {
    // Setup filesystem watch of unsquashfs progress file.
    this->monitorProgressFiles();
  } else if (hooks_pack_->type == HookType::InChroot) {
    if (!ChrootCopyHooks()) {
      qCritical() << "Failed to copy hooks into /target";
      emit this->errorOccurred();
      return;
    }
  }

  // Run hooks one by one.
  hooks_pack_->current_hook = -1;
  this->runNextHook();
}

void HooksManager::monitorProgressFiles() {
  qDebug() << "monitorProgressFiles()";
  // Remove old progress files first.
  QFile::remove(kUnsquashfsProgressFile);
  unsquashfs_timer_->start();
}

void HooksManager::handleRunHooks() {
  qDebug() << "handleRunHooks()";
  unsquashfs_timer_->setInterval(kReadUnsquashfsInterval);

  // First copy hooks from system and oem folder into the same folder.
  if (!CopyHooks()) {
    qCritical() << "Copy hooks failed!";
    emit this->errorOccurred();
    return;
  }

  HooksPack* before_chroot = new HooksPack();
  HooksPack* in_chroot = new HooksPack();
  HooksPack* after_chroot = new HooksPack();

  before_chroot->init(HookType::BeforeChroot, kBeforeChrootStartVal,
                      kBeforeChrootEndVal, in_chroot);
  in_chroot->init(HookType::InChroot, kInChrootStartVal,
                  kInChrootEndVal, after_chroot);
  after_chroot->init(HookType::AfterChroot, kAfterChrootStartVal,
                     kAfterChrootEndVal, nullptr);

  hooks_pack_ = before_chroot;
  this->runHooksPack();
}

void HooksManager::handleReadUnsquashfsTimeout() {
  // Read progress value and notify UI thread.
  const int val = ReadProgressValue(kUnsquashfsProgressFile);
  const int progress = kBeforeChrootStartVal +
      (kBeforeChrootEndVal - kBeforeChrootStartVal) * val / 100;
  if (hooks_pack_ && hooks_pack_->type == HookType::BeforeChroot) {
    emit this->processUpdate(progress);
  } else {
    unsquashfs_timer_->stop();
  }
}

void HooksManager::onHooksManagerFinished() {
  // Release hooks pack
  while (hooks_pack_) {
    HooksPack* next_hooks_pack = hooks_pack_->next;
    delete hooks_pack_;
    hooks_pack_ = next_hooks_pack;
  }

  // Stop unsquashfs progress file monitor
  if (unsquashfs_timer_->isActive()) {
    unsquashfs_timer_->stop();
  }
}

void HooksManager::onHookFinished(bool ok) {
  if (!ok) {
    const QString hook = hooks_pack_->hooks.at(hooks_pack_->current_hook);
    qCritical() << "Hook failed:" << GetFileName(hook);
    emit this->errorOccurred();
    return;
  }

  this->runNextHook();
}

}  // namespace installer
