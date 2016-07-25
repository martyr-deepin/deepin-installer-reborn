// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/models/language_list_model.h"

#include <QDebug>

namespace ui {

LanguageListModel::LanguageListModel(QObject* parent)
    : QAbstractListModel(parent),
      lang_list_() {
  this->setObjectName(QStringLiteral("language_list_model"));

  // TODO(xushaohua): Move to init() function.
  lang_list_ = service::GetLanguageList();
}

int LanguageListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return lang_list_.length();
}

QVariant LanguageListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  return lang_list_.at(index.row()).local_name;
}

}  // namespace ui