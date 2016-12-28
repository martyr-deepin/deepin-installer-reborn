// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
