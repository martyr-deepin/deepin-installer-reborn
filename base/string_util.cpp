// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/string_util.h"

#include <QRegExp>

namespace base {

QString RegexpLabel(const QString& pattern, const QString& str) {
  QRegExp reg(pattern);
  if (reg.indexIn(str, 0) != -1) {
    return reg.cap(1);
  } else {
    return QString();
  }
}

}  // namespace base