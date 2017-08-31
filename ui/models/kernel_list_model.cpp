// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/kernel_list_model.h"

namespace installer {

KernelListModel::KernelListModel(QObject* parent)
    : QAbstractListModel(parent),
      kernel_list_(GetKernelList()) {
  this->setObjectName("kernel_list_model");
}

QVariant KernelListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return kernel_list_.items.at(index.row());
  } else {
    return QVariant();
  }
}

int KernelListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return kernel_list_.items.length();
}

QString KernelListModel::getTitle() const {
  return kernel_list_.title;
}

QModelIndex KernelListModel::getSelectedKernel() const {
  const int selected = kernel_list_.selected;
  if (selected < 0 || selected >= kernel_list_.items.length()) {
    return QModelIndex();
  } else {
    return this->index(selected);
  }
}

QString KernelListModel::getKernelVersion(const QModelIndex& index) const {
  const int row = index.row();
  if (index.isValid() && row < kernel_list_.items.length()) {
    return kernel_list_.items.at(row);
  } else {
    return QString();
  }
}

}  // namespace installer