// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/inner/partition_usage.h"

#include <QProcess>
#include <QRegExp>

#include "base/command.h"
#include "base/string_util.h"
#include "service/partition_manager_structs.h"
#include "sysinfo/proc_swaps.h"

namespace service {

namespace {

qint64 ParseBtrfsUnit(const QString& value) {
  const float pref = QString(value).remove(QRegExp("K|M|G|T|P|Bib")).toFloat();
  if (value.contains("K")) {
    return static_cast<qint64>(pref * 1024);
  }
  if (value.contains("M")) {
    return static_cast<qint64>(pref * (1 << 20));
  }
  if (value.contains("G")) {
    return static_cast<qint64>(pref * (qint64(1) << 30));
  }
  if (value.contains("T")) {
    return static_cast<qint64>(pref * (qint64(1) << 40));
  }
  if (value.contains("P")) {
    return static_cast<qint64>(pref * (qint64(1) << 50));
  }
  return 0;
}

}  // namespace

bool ReadBtrfsUsage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("btrfs", {"filesystem", "show", path}, output)) {
    return false;
  }
  QString total_str, used_str;
  for (const QString& line : output.split('\n')) {
    if (line.contains(path)) {
      total_str = line.split(' ').at(3);
    } else if (line.contains("Total devices")) {
      used_str = line.split(' ').at(6);
    }
  }
  total = ParseBtrfsUnit(total_str);
  freespace = total - ParseBtrfsUnit(used_str);
  return true;
}

bool ReadExt2Usage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("dumpe2fs", {"-h", path}, output)) {
    return false;
  }

  int block_size = 0;
  qint64 total_blocks = 0;
  qint64 free_blocks = 0;
  for (const QString& line : output.split('\n')) {
    if (line.contains("Block count:")) {
      const QString m = base::RegexpLabel("Block count:\\s+(\\d+)", line);
      if (!m.isEmpty()) {
        total_blocks = m.toLongLong();
      }
    } else if (line.contains("Free blocks:")) {
      const QString m = base::RegexpLabel("Free blocks:\\s+(\\d+)", line);
      if (!m.isEmpty()) {
        free_blocks = m.toLongLong();
      }
    } else if (line.contains("Block size:")) {
      const QString m = base::RegexpLabel("Block size:\\s+(\\d+)", line);
      if (!m.isEmpty()) {
        block_size = m.toInt();
      }
    }
  }

  freespace = block_size * free_blocks;
  total = block_size * total_blocks;
  return true;
}

bool ReadFat16Usage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("dosfsck", {"-n", "-v", path}, output)) {
    return false;
  }

  int cluster_size = 0;
  qint64 start_byte = 0;
  qint64 total_clusters = 0;
  qint64 used_clusters = 0;

  for (const QString& line : output.split('\n')) {
    if (line.contains("bytes per cluster")) {
      cluster_size = line.trimmed().split(' ').at(0).trimmed().toInt();
    } else if (line.contains("Data area starts at")) {
      start_byte = line.split(' ').at(5).toLongLong();
    } else if (line.contains(path)) {
      const QStringList parts = line.split(' ').at(3).split('/');
      total_clusters = parts.at(1).toLongLong();
      used_clusters = parts.at(0).toLongLong();
    }
  }

  total = total_clusters * cluster_size;
  freespace = total - start_byte + used_clusters * cluster_size;
  return true;
}

bool ReadJfsUsage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("jfs_fsck", {"-n", path}, output)) {
    return false;
  }
  qint64 used = 0;
  for (const QString& line : output.split('\n')) {
    if (line.contains("kilobytes total disk space")) {
      total = line.split(' ').at(0).trimmed().toLongLong();
    } else if (line.contains("kilobytes are available for use")) {
      used = line.split(' ').at(0).trimmed().toLongLong();
    }
  }
  freespace = total - used;
  return true;
}

bool ReadLinuxSwapUsage(const QString& path, qint64& freespace, qint64& total) {
  const sysinfo::SwapItemList swap_items = sysinfo::ParseSwaps();
  for (const sysinfo::SwapItem& item : swap_items) {
    if (item.filename == path) {
      total = item.size;
      freespace = item.size - item.used;
      return true;
    }
  }
  return false;
}

bool ReadNTFSUsage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("ntfsinfo", {"-mf", path}, output)) {
    return false;
  }
  int cluster_size = 0;
  qint64 free_clusters = 0;
  qint64 total_clusters = 0;
  for (const QString& line : output.split('\n')) {
    if (line.contains("Cluster Size:")) {
      const QString m = base::RegexpLabel("Cluster Size:\\s+(\\d+)", line);
      if (!m.isEmpty()) {
        cluster_size = m.toInt();
      }
    } else if (line.contains("Volume Size in Clusters:")) {
      const QString m = base::RegexpLabel("Volume Size in Clusters:\\s+(\\d+)",
                                          line);
      if (!m.isEmpty()) {
        total_clusters = m.toLongLong();
      }
    } else if (line.contains("Free Clusters:")) {
      const QString m = base::RegexpLabel("Free Clusters:\\s+(\\d+)", line);
      if (!m.isEmpty()) {
        free_clusters = m.toLongLong();
      }
    }
  }
  freespace = cluster_size * free_clusters;
  total = cluster_size * total_clusters;
  return true;
}

bool ReadReiser4Usage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("debugfs.reiser4",
                      {"--force", "--yes", QString("--label '%1'").arg(path)},
                      output)) {
    return false;
  }

  int block_size = 0;
  qint64 free_blocks = 0;
  qint64 total_blocks = 0;
  for (const QString& line : output.split('\n')) {
    if (line.contains("blksize:")) {
      block_size = line.split(':').at(1).trimmed().toInt();
    } else if (line.contains("blocks:")) {
      total_blocks = line.split(':').at(1).trimmed().toLongLong();
    } else if (line.contains("free blocks:")) {
      free_blocks = line.split(':').at(1).trimmed().toLongLong();
    }
  }
  total = total_blocks * block_size;
  freespace = free_blocks * block_size;
  return true;
}

bool ReadReiserfsUsage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("debugreiserfs", {"-d", path}, output)) {
    return false;
  }
  for (const QString& line : output.split('\n')) {
    if (line.contains("Count of blocks on the device")) {
      total = line.split(' ').last().trimmed().toLongLong();
    } else if (line.contains("Free blocks (count of blocks")) {
      freespace = line.split(' ').last().trimmed().toLongLong();
    }
  }
  return true;
}

bool ReadXfsUsage(const QString& path, qint64& freespace, qint64& total) {
  QString output;
  if (!base::SpawnCmd("xfs_db",
                      {"-c 'sb 0'", "-c 'print dblocks'",
                       "-c 'print fdblocks'", "-r ", path},
                      output)) {
    return false;
  }
  for (const QString& line : output.split('\n')) {
    if (line.contains("fdblocks")) {
      freespace = line.split(' ').last().trimmed().toLongLong();
    } else if (line.contains("dblocks")) {
      total = line.split(' ').last().toLongLong();
    }
  }
  return true;
}

}  // namespace service