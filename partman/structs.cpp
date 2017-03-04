// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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