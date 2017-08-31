// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_VERSION_LIST_MODEL_H_
#define INSTALLER_UI_MODELS_VERSION_LIST_MODEL_H_

#include <QAbstractListModel>

#include "service/kernel_list.h"

namespace installer {

// Model used in SelectKernelFrame to hold kernel type.
class KernelListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit KernelListModel(QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get title name.
  QString getTitle() const;

  // Get index of default selected kernel.
  // Returns -1 if not set.
  QModelIndex getSelectedKernel() const;

  // Get kernel version at |index|.
  QString getKernelVersion(const QModelIndex& index) const;

 private:
  KernelList kernel_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_VERSION_LIST_MODEL_H_
