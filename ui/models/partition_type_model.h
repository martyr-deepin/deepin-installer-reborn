/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H
#define INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H

#include <QAbstractListModel>

namespace installer {

// Model to display available partition type in NewPartitionFrame.
class PartitionTypeModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit PartitionTypeModel(QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Get index of logical partition.
  int getLogicalIndex() const;

  // Get index of primary partition.
  int getPrimaryIndex() const;

  // Check whether selected item at |index| is logical partition.
  bool isLogical(int index) const;

  // Check whether selected item at |index| is primary partition.
  bool isPrimary(int index) const;

 public slots:
  // Reset status. Both "primary partition" and "logical partition" are
  // available now.
  void reset();

  // Set visibility of "logical partition" option.
  void setLogicalVisible(bool visible);

  // Set visibility of "primary partition" option.
  void setPrimaryVisible(bool visible);

 private:
  bool is_logical_visible_;
  bool is_primary_visible_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_PARTITION_TYPE_MODEL_H
