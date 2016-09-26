// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_BASE_STRING_UTIL_H
#define DEEPIN_INSTALLER_REBORN_BASE_STRING_UTIL_H

#include <QString>

namespace base {

// Parse a pattern |pattern| from string |str| and returns matched substring
// or an empty string.
// Note that a match group shall be specified in |pattern|.
QString RegexpLabel(const QString& pattern, const QString& str);

}  // namespace base

#endif  // DEEPIN_INSTALLER_REBORN_BASE_STRING_UTIL_H
