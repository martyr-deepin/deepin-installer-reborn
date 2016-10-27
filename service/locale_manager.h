// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_LOCALE_MANAGER_H
#define INSTALLER_LOCALE_MANAGER_H

#include <QObject>
class QThread;

namespace installer {

class WiFiInspectWorker;

class LocaleManager : public QObject {
  Q_OBJECT

 public:
  explicit LocaleManager(QObject* parent = nullptr);
  ~LocaleManager();

  void update();

 signals:
  void timezoneUpdated(const QString& timezone);

 private:
  WiFiInspectWorker* wifi_inspect_worker_ = nullptr;
  QThread* wifi_inspect_thread_ = nullptr;
  QThread* locale_request_thread_ = nullptr;

 private slots:
  // Convert regdomain to locale.
  void onRegdomainUpdated(const QString& regdomain);
};

} // namespace installer

#endif  // INSTALLER_LOCALE_MANAGER_H
