// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_LOCALE_REQUEST_WORKER_H
#define INSTALLER_LOCALE_REQUEST_WORKER_H

#include <QObject>

namespace installer {

class LocaleRequestWorker : public QObject {
  Q_OBJECT

 public:
  explicit LocaleRequestWorker(QObject* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_LOCALE_REQUEST_WORKER_H
