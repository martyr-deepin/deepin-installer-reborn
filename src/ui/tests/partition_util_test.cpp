/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include <QDebug>
#include <QRegularExpression>
#include <QString>

qint64 ParsePartitionSize(const QString& size, qint64 device_length) {
  const qint64 kKibiByte = 1024;
  const qint64 kMebiByte = kKibiByte * kKibiByte;
  const qint64 kGibiByte = kMebiByte * kKibiByte;
  const qint64 kTebiByte = kGibiByte * kKibiByte;
  const qint64 kPebiByte = kTebiByte * kKibiByte;
  const qint64 kExbiByte = kPebiByte * kKibiByte;
  Q_UNUSED(kExbiByte);

  QRegularExpression pattern("(\\d+)(mib|gib|%)",
                             QRegularExpression::CaseInsensitiveOption);
  const QRegularExpressionMatch match = pattern.match(size);
  if (match.hasMatch()) {
    bool ok;
    const QString size_value = match.captured(1);
    const int size = size_value.toInt(&ok, 10);
    if (!ok) {
      qDebug() << "Failed to parse size," << size_value;
      return -1;
    }

    const QString unit = match.captured(2).toLower();
    if (unit == "mib") {
      return size * kMebiByte;
    } else if (unit == "gib") {
      return size * kGibiByte;
    } else if (unit == "%") {
      return size * device_length;
    } else {
      return -1;
    }
  }
  return -1;
}

int main(void) {
  const QString size_value = "1Mib";
  const qint64 size = ParsePartitionSize(size_value, 100);
  qDebug() << "size: " << size;
}
