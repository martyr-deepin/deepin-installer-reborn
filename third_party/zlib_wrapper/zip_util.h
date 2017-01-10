// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_THIRD_PARTY_ZLIB_WRAPPER_ZIP_UTIL_H
#define INSTALLER_THIRD_PARTY_ZLIB_WRAPPER_ZIP_UTIL_H

#include <QByteArray>

// Compress |input| with gzip, saved in |output|.
// |level| is compressing level, 9 means best, 1 means fastest.
bool GzipCompress(const QByteArray& input, QByteArray& output, int level);

// Decompress |input| coding with gzip, saved in |output|.
bool GzipDecompress(const QByteArray& input, QByteArray& output);

#endif  // INSTALLER_THIRD_PARTY_ZLIB_WRAPPER_ZIP_UTIL_H
