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
  const QByteArray localMsg = msg.toLocal8Bit();
  const char* filename =
      QFileInfo(context.file).fileName().toUtf8().constData();
  switch (type) {
    case QtDebugMsg:
      fprintf(stdout, "[Debug] [%s %u]: %s\n", filename, context.line,
              localMsg.constData());
      break;
    case QtInfoMsg:
      fprintf(stdout, "[Info] [%s:%u]: %s\n", filename, context.line,
              localMsg.constData());
      break;
    case QtWarningMsg:
      fprintf(stdout, "[Warning] [%s %u]: %s\n", filename, context.line,
              localMsg.constData());
      break;
    case QtCriticalMsg:
      fprintf(stderr, "[Error] [%s %u]: %s\n", filename, context.line,
              localMsg.constData());
      break;
    case QtFatalMsg:
      fprintf(stderr, "[Fatal] [%s %u]: %s\n", filename, context.line,
              localMsg.constData());
      abort();
  }
}

}  // namespace

QString GetLogFilepath() {
  const QString var_log = QStringLiteral("/var/log/%1").arg(kLogFileName);
  QFile var_log_file(var_log);
  if (var_log_file.isWritable()) {
    if (var_log_file.exists()) {
      var_log_file.remove();
    }
    return var_log;
  }

  const QString tmp_log = QStringLiteral("/tmp/%1").arg(kLogFileName);
  QFile tmp_log_file(tmp_log);
  if (tmp_log_file.exists()) {
    tmp_log_file.remove();
  }
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
