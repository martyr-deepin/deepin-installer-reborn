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