// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_LOCALE_MANAGER_H
#define INSTALLER_LOCALE_MANAGER_H

#include <QObject>

namespace installer {

class LocaleManager : public QObject {
  Q_OBJECT

 public:
  explicit LocaleManager(QObject* parent = nullptr);

  void update();

 signals:
  void localeUpdated(const QString& locale);
  void timezoneUpdated(const QString& timezone);
};

} // namespace installer

#endif  // INSTALLER_LOCALE_MANAGER_H
