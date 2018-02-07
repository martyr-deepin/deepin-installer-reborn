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

#ifndef INSTALLER_SYSINFO_ISO3166_H
#define INSTALLER_SYSINFO_ISO3166_H

#include <QList>
#include <QString>

namespace installer {

struct CountryEntry {
  QString alpha2 = "";  // Country code with 2 chars, like CN, US.
  QString alpha3 = "";  // Country code with 3 chars, Like CHN, USA.
  QString name = "";
  int numberic = 0;
  QString official_name = "";
};
typedef QList<CountryEntry> CountryEntries;

// Read all countries in the ISO 3166 standard.
CountryEntries GetCountryEntries();

// Get country entry index with country code |cc| in CountryEntries |entries|.
// |cc| might be alpha2 or alpha3 in CountryEntry.
// Might return -1 if not found.
int GetCountryEntryIndex(const CountryEntries& entries, const QString& cc);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_ISO3166_H
