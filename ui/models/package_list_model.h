// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_PACKAGE_LIST_MODEL_H_
#define INSTALLER_UI_MODELS_PACKAGE_LIST_MODEL_H_

#include <QAbstractListModel>

#include "service/package_list.h"

namespace installer {

// Model used in PackageListFrame to hold package names.
class PackageListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit PackageListModel(QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get title name.
  QString getTitle() const;

  // Get package name at |index|.
  QString getPackage(const QModelIndex& index) const;

  // Get default selected packages.
  QModelIndexList getSelectedPackages() const;

 private:
  PackageList package_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PACKAGE_LIST_MODEL_H_
