// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <QDebug>
#include <QRegExp>

#include "base/file_util.h"

const int kMaxEvents = 200;

// Timeout is 100ms.
const int kTimeout = 100;

int main(int argc, char* argv[]) {
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  int epoll_fd;

  // `events` is used to handle received epoll events.
  struct epoll_event events[kMaxEvents];

  // `register_event` is used to register stdin to epoll system.
  struct epoll_event register_event;
  int event_num;

  char* line = NULL;
  size_t line_len = 0;
  ssize_t num_read = 0;

  QString q_str_line;
  QRegExp progress_reg("(\\d{1,3})%");

  // Only watch stdin.
  epoll_fd = epoll_create(1);
  if (epoll_fd <= 0) {
    qWarning() << "Failed to create epoll file descriptor";
    exit(EXIT_FAILURE);
  }

  // Monitors file read and set to ET mode.
  register_event.data.fd = STDIN_FILENO;
  register_event.events = EPOLLIN | EPOLLET;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &register_event);

  // Read events
  while ((event_num = epoll_wait(epoll_fd, events, kMaxEvents, kTimeout)) > 0) {
    // Read events.
    for (int i = 0 ; i < event_num; ++i) {
      if (events[i].data.fd == STDIN_FILENO) {
        while ((num_read = getline(&line, &line_len, stdin)) != -1) {
          q_str_line = line;
          if (progress_reg.indexIn(q_str_line, 0) != -1) {
//            qDebug() << "progress:" << progress_reg.cap(1);
            QString cap = progress_reg.cap(1);
            cap += "\n";
            write(STDOUT_FILENO, cap.toStdString().c_str(), cap.length());
          }
        }
      }
    }
  }

  exit(EXIT_SUCCESS);
}