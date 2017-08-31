// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/package_list_model.h"

namespace installer {

PackageListModel::PackageListModel(QObject* parent)
    : QAbstractListModel(parent),
      package_list_(GetPackageList()) {
  this->setObjectName("package_list_model");
}

QVariant PackageListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return package_list_.items.at(index.row());
  } else {
    return QVariant();
  }
}

int PackageListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return package_list_.items.length();
}

QString PackageListModel::getTitle() const {
  return package_list_.title;
}

QString PackageListModel::getPackage(const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < package_list_.items.length()) {
    return package_list_.items.at(row);
  } else {
    return QString();
  }
}

QModelIndexList PackageListModel::getSelectedPackages() const {
  QModelIndexList list;

  // Get index of selected packages.
  for (const int item : package_list_.selected) {
    if (item > -1 && item < package_list_.items.length()) {
      list.append(this->index(item));
    }
  }

  return list;
}

}  // namespace installer