// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef NSTALLER_WIFI_INSPECT_WORKER_H
#define NSTALLER_WIFI_INSPECT_WORKER_H

#include <QObject>

namespace installer {

class WiFiInspectWorker : public QObject {
  Q_OBJECT

 public:
  explicit WiFiInspectWorker(QObject* parent = nullptr);

};

}  // namespace installer

#endif  // NSTALLER_WIFI_INSPECT_WORKER_H
