// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "language_list_model.h"

#include <QDebug>

namespace installer {

LanguageListModel::LanguageListModel(QObject* parent)
    : QAbstractListModel(parent),
      lang_list_() {
  this->setObjectName("language_list_model");

  // TODO(xushaohua): Move to init() function.
  lang_list_ = GetLanguageList();
}

LanguageListModel::LanguageListModel(bool append_empty, QObject* parent)
    : LanguageListModel(parent) {
  if (append_empty) {
    // Append an empty language item.
    LanguageItem empty_lang;
    empty_lang.name = "Do not set";
    empty_lang.local_name = tr("Do not set");
    empty_lang.locale = "";
    empty_lang.lc_all = "";
    lang_list_.prepend(empty_lang);
  }
}

QVariant LanguageListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return lang_list_.at(index.row()).local_name;
  } else {
    return QVariant();
  }
}

int LanguageListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return lang_list_.length();
}

LanguageItem LanguageListModel::languageItemAt(int index) const {
  if (index >= 0 && index < lang_list_.length()) {
    return lang_list_.at(index);
  } else {
    return LanguageItem();
  }
}

LanguageItem LanguageListModel::languageItemAt(const QModelIndex& index) const {
  if (index.isValid()) {
    return lang_list_.at(index.row());
  } else {
    return LanguageItem();
  }
}

QModelIndex LanguageListModel::localeIndex(const QString& locale) const {
  for (int index = 0; index < lang_list_.length(); index++) {
    if (lang_list_.at(index).locale == locale) {
      return this->index(index);
    }
  }
  return QModelIndex();
}

}  // namespace installer