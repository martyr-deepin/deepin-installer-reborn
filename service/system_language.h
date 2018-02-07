/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_SERVICE_SYSTEM_LANGUAGE_H
#define INSTALLER_SERVICE_SYSTEM_LANGUAGE_H

#include <QList>
#include <QString>

namespace installer {

struct LanguageItem {
  QString name;  // English name.
  QString locale;  // Language name, like en_US.
  QString local_name;  // Localized name.
  QString timezone;  // Default timezone, might be empty.
};

typedef QList<LanguageItem> LanguageList;

LanguageList GetLanguageList();

// Generate locale based on |locale|.
// Call `locale-gen` to update system locales, or else setlocale() will fail
// in future.
void GenerateLocale(const QString& locale);

}  // namespace installer

#endif  // INSTALLER_SERVICE_SYSTEM_LANGUAGE_H
