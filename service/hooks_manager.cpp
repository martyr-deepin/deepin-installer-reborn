// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/hooks_manager.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <QDebug>
#include <QThread>

namespace service {

namespace {

const char kTargetDir[] = "/target";

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  return true;
}

}  // namespace

HooksManager::HooksManager(QObject* parent)
    : QObject(parent),
      chroot_fd_(0) {
  this->setObjectName(QStringLiteral("hooks_manager"));

  connect(this, &HooksManager::runHooks,
          this, &HooksManager::handleRunHooks);
}

QStringList HooksManager::listHooks(HooksManager::HookType hook_type) {
  QStringList hooks;
  return hooks;
}

bool HooksManager::runHooksPack(HooksManager::HookType hook_type,
                                int progress_begin, int progress_end) {
  qDebug() << "[HooksManager]::runHooksPack():" << progress_begin
           << progress_end;
  const QStringList hooks = listHooks(hook_type);

  if (hook_type == HookType::InChroot) {
    if (this->enterChroot()) {
      // errorOccurred() signal is emitted in enterChroot() already.
      return false;
    }
  }

  foreach (const QString& hook, hooks) {
    if (!RunHook(hook)) {
      const QString msg =
          QString("[HooksManager]::runHooksPack() job failed: %1").arg(hook);
      qCritical() << msg;
      emit this->errorOccurred(msg);
      return false;
    }
    // TODO(xushaohua): emit processUpdate() signal.
  }

  if (hook_type == HookType::InChroot) {
    return this->leaveChroot();
  }
  return true;
}

bool HooksManager::enterChroot() {
  chroot_fd_ = open("/", O_RDONLY);
  if (chroot_fd_ == -1) {
    const QString msg =
        QStringLiteral("[HooksManager]::enterChroot() failed to '/'!");
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }

  if (chdir(kTargetDir) != 0) {
    const QString msg = QString(
        "[HooksManager]::enterChroot() failed to call chdir(%s)").arg(
        kTargetDir);
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }

  if (chroot(kTargetDir) != 0) {
    const QString msg =
        QStringLiteral("[HooksManager]::enterChroot() failed to call chroot()");
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }

  if (chdir("/") != 0) {
    const QString msg =
        QStringLiteral("[HooksManager]::enterChroot() failed to call chdir(/)");
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }
  return true;
}

bool HooksManager::leaveChroot() {
  if (fchdir(chroot_fd_) != 0) {
    const QString msg = QString(
        "[HooksManager]::leaveChroot() fchdir(%1) failed").arg(chroot_fd_);
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }
  // Chroot to host environment.
  if (chroot(".") != 0) {
    const QString msg =
        QStringLiteral("[HooksManager]::leaveChroot() failed to call chroot()");
    qCritical() << msg;
    emit this->errorOccurred(msg);
    return false;
  }
  return true;
}

void HooksManager::handleRunHooks() {
  // Run hooks one by one, if some job fails, returns immediately.

  if (!runHooksPack(HookType::BeforeChroot, 0, 60)) {
    qWarning() << "handleRunHooks() error occurs in Before_Chroot";
    return;
  }
  if (!runHooksPack(HookType::InChroot, 60, 80)) {
    qWarning() << "handleRunHooks() error occurs in In_Chroot";
    return;
  }
  if (!runHooksPack(HookType::AfterChroot, 80, 100)) {
    qWarning() << "handleRunHooks() error occurs in After_Chroot";
  }
}

}  // namespace service