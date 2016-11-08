// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <QDebug>
#include <QRegExp>

#include "base/file_util.h"

namespace {

void WriteProgress(const QString& output_file, const QString& progress) {
  qDebug() << "WriteProgress:" << output_file << progress;
  if (output_file.isEmpty()) {
    write(STDOUT_FILENO, progress.toUtf8().constData(),
          static_cast<size_t>(progress.length()));
  } else {
    installer::WriteTextFile(output_file, progress);
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  QString output_file;
  if (argc == 2) {
    output_file = argv[1];
    installer::CreateParentDirs(output_file);
  } else {
    qWarning() << "Output file not set, use stdout instead!";
  }

  QRegExp progress_reg("(\\d{1,3})%");
  QString q_str_line;
  QString progress;

  size_t kBufSize = 128;
  char buf[kBufSize];
  ssize_t num_read = read(STDIN_FILENO, buf, kBufSize);
  while (num_read > 0) {
    q_str_line = buf;

    if (progress_reg.indexIn(q_str_line, 0) != -1) {
      progress = progress_reg.cap(1);
      WriteProgress(output_file, progress);
    }

    num_read = read(STDIN_FILENO, buf, kBufSize);
  }

  // `unsquashfs` exits successfully without printing 100% progress value.
  // So we check by hand.
  if (progress == "99") {
    WriteProgress(output_file, "100");
  }

  // TODO(xushaohua): Handles pipe error
  exit(EXIT_SUCCESS);
}