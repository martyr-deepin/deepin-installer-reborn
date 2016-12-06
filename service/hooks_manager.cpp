// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/hooks_manager.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QTimer>

#include "base/file_util.h"
#include "service/backend/hooks_pack.h"
#include "service/backend/hook_worker.h"

namespace installer {

namespace {

const int kBeforeChrootEndVal = 60;
const int kInChrootStartVal = 60;
const int kInChrootEndVal = 80;
const int kAfterChrootStartVal = 80;
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

  hook_worker_thread_->start();
  hook_worker_->moveToThread(hook_worker_thread_);

  this->initConnections();
}

HooksManager::~HooksManager() {
  // TODO(xushaohua): delete thread.
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
    // Run next hooks pack if it is not nullptr
    if (hooks_pack_ == nullptr) {
      emit this->finished();
    } else {
      this->runHooksPack();
    }
  } else {
    // Update progress, except before-chroot.
    if (hooks_pack_->type != HookType::BeforeChroot) {
      const int progress = hooks_pack_->progress_begin +
          (hooks_pack_->progress_end - hooks_pack_->progress_begin) /
              hooks_pack_->hooks.length() * hooks_pack_->current_hook;
      qDebug() << "processUpdate():" << progress;
      emit this->processUpdate(progress);
    }

    // Run next hook in current hooks pack.
    const QString hook(hooks_pack_->hooks.at(hooks_pack_->current_hook));
    qDebug() << "run hook:" << hook;
    emit hook_worker_->runHook(hook);
  }
}

void HooksManager::runHooksPack() {
  Q_ASSERT(hooks_pack_);
  if (hooks_pack_->type == HookType::BeforeChroot) {
    // Setup filesystem watch of unsquashfs progress file.
    this->monitorProgressFiles();
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

  HooksPack* before_chroot = new HooksPack();
  HooksPack* in_chroot = new HooksPack();
  HooksPack* after_chroot = new HooksPack();

  before_chroot->init(HookType::BeforeChroot, kBeforeChrootStartVal,
                      kBeforeChrootEndVal, false, in_chroot);
  in_chroot->init(HookType::InChroot, kInChrootStartVal,
                  kInChrootEndVal, true, after_chroot);
  after_chroot->init(HookType::AfterChroot, kAfterChrootStartVal,
                     kAfterChrootEndVal, false, nullptr);

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
    qCritical() << "Hook failed:"
                << hooks_pack_->hooks.at(hooks_pack_->current_hook);
    emit this->errorOccurred();
    return;
  }

  this->runNextHook();
}

}  // namespace installer
