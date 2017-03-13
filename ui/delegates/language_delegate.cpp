// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/language_delegate.h"

#include <QDebug>
#include <QFile>

namespace installer {

namespace {

const char kDefaultLanguageFile[] = I18N_DIR "/installer-en_US.qm";
const char kLanguageFileTpl[] = I18N_DIR "/installer-%1.qm";

}  // namespace

QString GetLocalePath(const QString& locale) {
  QString filepath = QString(kLanguageFileTpl).arg(locale);
  if (QFile::exists(filepath)) {
    return filepath;
  }

  // Get language region only. e.g. ru_RU => ru
  const int underscore_index = locale.indexOf('_');
  filepath = QString(kLanguageFileTpl).arg(locale.left(underscore_index));
  if (QFile::exists(filepath)) {
    return filepath;
  }

  return kDefaultLanguageFile;
}

}  // namespace installer