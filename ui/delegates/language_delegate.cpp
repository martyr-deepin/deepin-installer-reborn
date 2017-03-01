// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/language_delegate.h"

namespace installer {

namespace {

const char kDefaultLanguageFile[] = I18N_DIR "/en_US.qm";
const char kLanguageFileTpl[] = I18N_DIR "/installer-%1.qm";

}  // namespace

QString GetDefaultLocalePath() {
  return kDefaultLanguageFile;
}

QString GetLocalePath(const QString& locale) {
  return QString(kLanguageFileTpl).arg(locale);
}

}  // namespace installer