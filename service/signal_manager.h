// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_SERVICE_SIGNAL_MANAGER_H
#define DEEPIN_INSTALLER_SERVICE_SIGNAL_MANAGER_H

#include <QObject>

namespace service {

class SignalManager : public QObject {
  Q_OBJECT

 public:
  SignalManager();
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_SERVICE_SIGNAL_MANAGER_H
