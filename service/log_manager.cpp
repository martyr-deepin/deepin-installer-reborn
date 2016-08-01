// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/log_manager.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>

#include "sysinfo/users.h"

#include "third_party/CuteLogger/CuteLogger/ConsoleAppender.h"
#include "third_party/CuteLogger/CuteLogger/FileAppender.h"
#include "third_party/CuteLogger/CuteLogger/Logger.h"

namespace service {

namespace {

// Global log file descriptor.
int g_log_fd = 0;

// Defines log format.
const char kLogFormat[] =
    "%{time}{MM-dd, HH:mm:ss} [%{type:-7}] [%{file:-25} %{line}] %{message}\n";

const char kLogFileName[] = "deepin-installer-reborn.log";

// Define customized QDebug handler.
void MessageOutput(QtMsgType type, const QMessageLogContext& context,
                   const QString& msg) {
  Q_UNUSED(type);
  Q_UNUSED(context);
  if (write(STDOUT_FILENO, msg.toUtf8().constData(),
            static_cast<size_t>(msg.length())) > -1 &&
      write(STDOUT_FILENO, "\n", 1) > -1) {
    return;
  } else {
    perror("");
  }
  // To reduce duplicated log message, remove all flags.
//  switch (type) {
//    case QtDebugMsg: {
//      const QString content = QString("[Debug] [%1 %2]: %3\n")
//          .arg(filename).arg(context.line).arg(localMsg.constData());
//      write(STDOUT_FILENO, content.toUtf8().constData(), content.length());
//      break;
//    }
//    case QtInfoMsg: {
//      const QString content = QString("[Info] [%1 %2]: %3\n")
//          .arg(filename).arg(context.line).arg(localMsg.constData());
//      write(STDOUT_FILENO, content.toUtf8().constData(), content.length());
//      break;
//    }
//    case QtWarningMsg: {
//      const QString content = QString("[Warning] [%1 %2]: %3\n")
//          .arg(filename).arg(context.line).arg(localMsg.constData());
//      write(STDOUT_FILENO, content.toUtf8().constData(), content.length());
//      break;
//    }
//    case QtCriticalMsg: {
//      const QString content = QString("[Critical] [%1 %2]: %3\n")
//          .arg(filename).arg(context.line).arg(localMsg.constData());
//      write(STDOUT_FILENO, content.toUtf8().constData(), content.length());
//      break;
//    }
//    case QtFatalMsg: {
//      const QString content = QString("[Fatal] [%1 %2]: %3\n")
//          .arg(filename).arg(context.line).arg(localMsg.constData());
//      write(STDOUT_FILENO, content.toUtf8().constData(), content.length());
//      abort();
//    }
//  }
}

}  // namespace

QString GetLogFilepath() {
  if (sysinfo::HasRootPrivilege()) {
    return QString("/var/log/%1").arg(kLogFileName);
  }

  const QString tmp_log = QString("/tmp/%1").arg(kLogFileName);
  return tmp_log;
}

void InitLogService() {
  // TODO(xushaohua): Free console_appender and file_appender.
  ConsoleAppender* console_appender = new ConsoleAppender();
#ifndef NDEBUG
  console_appender->setDetailsLevel(Logger::Debug);
#else
  console_appender->setDetailsLevel(Logger::Warning);
#endif
  console_appender->setFormat(kLogFormat);
  logger->registerAppender(console_appender);

  FileAppender* file_appender = new FileAppender(GetLogFilepath());
#ifndef NDEBUG
  file_appender->setDetailsLevel(Logger::Debug);
#else
  file_appender->setDetailsLevel(Logger::Warning);
#endif
  file_appender->setFormat(kLogFormat);
  logger->registerAppender(file_appender);
}

void RedirectLogFile() {
  qInstallMessageHandler(MessageOutput);

  if (g_log_fd == 0) {
    const QString log_file = GetLogFilepath();
    g_log_fd = open(log_file.toStdString().c_str(),
                    O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (g_log_fd == -1) {
      qWarning() << "RedirectLogFile() failed to open " << log_file;
      g_log_fd = 0;
      return;
    }
    if (dup2(g_log_fd, STDOUT_FILENO) == -1 ||
        dup2(g_log_fd, STDERR_FILENO) == -1) {
      qWarning() << "RedirectLogFile() failed top redirect stdio";
    }
  }
}

void ShutdownLogService() {
  if (g_log_fd != 0) {
    close(g_log_fd);
  }
}

}  // namespace service
