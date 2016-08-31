// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H

namespace partman {

// This header file defines commonly used types and struct type in partman
// module.

typedef long long Sector;
typedef long long ByteValue;

// Size units defined in bytes
// See https://en.wikipedia.org/wiki/Kibibyte
const ByteValue kKibiByte = 1024;
const ByteValue kMebiByte = kKibiByte * kKibiByte;
const ByteValue kGibiByte = kMebiByte * kKibiByte;
//const ByteValue kTebiByte = kGibiByte * kKibiByte;
//const ByteValue kPebiByte = kTebiByte * kKibiByte;
//const ByteValue kExbiByte = kPebiByte * kKibiByte;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_STRUCTS_H
