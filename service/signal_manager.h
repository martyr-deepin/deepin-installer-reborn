// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_SIGNAL_MANAGER_H
#define INSTALLER_SERVICE_SIGNAL_MANAGER_H

#include <QObject>

namespace installer {

// This class is used to send signals across unrelated objects.
// It keeps a global instance.
class SignalManager : public QObject {
  Q_OBJECT

 public:
  static SignalManager* instance();

 signals:
  // PartitionFrame
  void autoPartDone(bool ok);
  void manualPartDone(bool ok);

  // Emitted when |locale| is selected in select-language-frame
  void languageSelected(const QString& locale);

 private:
  SignalManager();
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_SIGNAL_MANAGER_H
