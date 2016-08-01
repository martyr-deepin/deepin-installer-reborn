// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_SYSTEM_LANGUAGE_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_SYSTEM_LANGUAGE_H

#include <QList>
#include <QString>

namespace service {

struct LanguageItem {
  QString locale;  // Locale name, like en_US.
  QString name;  // English name.
  QString local_name;  // Localized name.
};

typedef QList<LanguageItem> LanguageList;

LanguageList GetLanguageList();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_SYSTEM_LANGUAGE_H
