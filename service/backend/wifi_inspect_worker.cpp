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

#include "service/backend/wifi_inspect_worker.h"

#include <QDebug>
#include <QHash>
#include <QRegularExpression>

#include "base/command.h"

namespace installer {

namespace {

// Number of beacon packets to capture before exit.
const char kTsharkPackets[] = "200";

// Time in seconds before exit.
const char kTsharkDuration[] = "10";

const char kScanRegdomainFile[] = TOOLS_DIR "/scan_regdomain.sh";

// Read output of scan_regdomain.sh and extract country code.
QStringList ParseRegdomainList(const QString& text) {
  QHash<QString, QString> regdomain_pair;
  // Matches <source_mac> <regdomain> pair
  const QString pattern("^([a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2}:[a-z0-9]{2}"
                        ":[a-z0-9]{2}:[a-z0-9]{2})\\s([A-Z]{2})$");
  QRegularExpression reg(pattern, QRegularExpression::MultilineOption);
  QRegularExpressionMatch match;
  for (const QString& line : text.split('\n')) {
    match = reg.match(line);
    if (match.hasMatch()) {
      const QString cc = match.captured(2);
      // Ignores empty regdomain.
      if (!cc.isEmpty()) {
        const QString mac = match.captured(1);
        regdomain_pair.insert(mac, cc);
      }
    }
  }
  return regdomain_pair.values();
}

// Returns item which has the most frequency in the |list|.
QString LargestCounted(const QStringList& list) {
  QString curr_key, last_key;
  int curr_count = 0;
  int last_count = 0;
  const int list_len = list.length();
  for (int i = 0; i < list_len; i++) {
    curr_key = list.at(i);
    curr_count = 0;
    for (int j = i; j < list_len; j++) {
      if (list.at(j) == curr_key) {
        curr_count++;
      }
    }
    if (curr_count > last_count) {
      last_key = curr_key;
      last_count = curr_count;
    }
  }
  return last_key;
}

}  // namespace

WiFiInspectWorker::WiFiInspectWorker(QObject* parent) : QObject(parent) {
  this->setObjectName("wifi_inspect_worker");

  connect(this, &WiFiInspectWorker::scan,
          this, &WiFiInspectWorker::doScanning);
}

void WiFiInspectWorker::doScanning() {
  // First enable monitor mode.
  // Then capture packets with tshark.
  // At last, disable monitor mode.
  QString out, err;
  if (RunScriptFile({kScanRegdomainFile, kTsharkPackets, kTsharkDuration},
                    out, err)) {
    QStringList list = ParseRegdomainList(out);
    if (!list.isEmpty()) {
      // Get the most used country code.
      const QString regdomain = LargestCounted(list);
      if (!regdomain.isEmpty()) {
        emit this->regdomainUpdated(regdomain);
      } else {
        qWarning() << "Regdomain is empty!";
      }
    } else {
      qWarning() << "Scanning regdomain failed, list is empty!" << err;
    }
  } else {
    qWarning() << "Scanning regdomain failed!" << out << err;
  }
}

}  // namespace installer
