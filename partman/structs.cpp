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

#include "partman/structs.h"

namespace installer {

QDebug& operator<<(QDebug& debug, const OsType& os_type) {
  QString os;
  switch (os_type) {
    case OsType::Linux: {
      os = "Linux";
      break;
    }
    case OsType::Windows: {
      os = "Windows";
      break;
    }
    case OsType::Mac: {
      os = "MacOs";
      break;
    }
    default: {
      // pass
    }
  }
  debug << os;
  return debug;
}

QDebug& operator<<(QDebug& debug, const PartitionTableType& table_type) {
  debug << GetPartTableName(table_type);
  return debug;
}

const char* GetPartTableName(PartitionTableType table_type) {
  switch (table_type) {
    case PartitionTableType::GPT: {
      return  "GPT";
    }
    case PartitionTableType::MsDos: {
      return "MsDos";
    }
    case PartitionTableType::Others: {
      return "Others";
    }
    case PartitionTableType::Unknown: {
      return "Unknown";
    }
    default: {
      return "Unknown";
    }
  }
}

}  // namespace installer