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

#include "service/hooks_manager.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QTimer>
#include <QDateTime>

#include "base/file_util.h"
#include "base/thread_util.h"
#include "service/backend/hooks_pack.h"
#include "service/backend/hook_worker.h"
#include "service/settings_name.h"
#include "service/settings_manager.h"

namespace installer {

namespace {

const int kBeforeChrootEndVal = 60;
const int kInChrootStartVal = kBeforeChrootEndVal;
const int kInChrootEndVal = 85;
const int kAfterChrootStartVal = kInChrootEndVal;
const int kAfterChrootEndVal = 100;

const char kUnsquashfsProgressFile[] = "/dev/shm/unsquashfs_progress";
// Interval to read unsquashfs progress file, 5000ms.
const int kReadUnsquashfsInterval = 5000;

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
      unsquashfs_timer_(new QTimer(this)),
      lastRunTime(0) {
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
    const QString& hook_name { GetFileName(hook) };
    QDateTime datetime = QDateTime::currentDateTime();
    if (enableScriptAnalyze) {
      const qlonglong runTime = datetime.toMSecsSinceEpoch() - lastRunTime;
      scriptRunTimeList << std::pair<QString, qlonglong>(hook_name, runTime);
      lastRunTime = datetime.toMSecsSinceEpoch();
    }
    qDebug() << QString("run hook: %1 at %2").arg(GetFileName(hook)).arg(datetime.toString("hh:mm:ss"));
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
  enableScriptAnalyze = GetSettingsBool(kEnableAnalysisScriptTime);
  lastRunTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

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

  if (enableScriptAnalyze) {
      qlonglong allTime { 0 };
      for (const std::pair<QString, qlonglong> record : scriptRunTimeList) {
          qDebug() << QString("run %1 used %2")
                          .arg(record.first)
                          .arg(QDateTime::fromMSecsSinceEpoch(record.second)
                                   .toString("hh:mm:ss"));
          allTime += record.second;
      }
      qDebug() << QString("All time is %1")
                      .arg(QDateTime::fromMSecsSinceEpoch(allTime).toString("hh:mm:ss"));
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
