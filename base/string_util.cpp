// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/string_util.h"

#include <QRegularExpression>

namespace installer {

QString Base64Decode(const QString& content) {
  return QByteArray::fromBase64(content.toLocal8Bit());
}

QString Base64Encode(const QString& content) {
  return content.toLocal8Bit().toBase64();
}

QString RegexpLabel(const QString& pattern, const QString& str) {
  QRegularExpression reg(pattern, QRegularExpression::MultilineOption);
  QRegularExpressionMatch match = reg.match(str);
  if (match.hasMatch()) {
    return match.captured(1);
  } else {
    return QString();
  }
}

}  // namespace installer