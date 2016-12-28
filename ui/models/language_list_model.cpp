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