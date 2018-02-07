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

#ifndef INSTALLER_OEM_OEM_WINDOW_H
#define INSTALLER_OEM_OEM_WINDOW_H

#include <QFrame>
class QListView;

namespace installer {

class OemSettingsItemView;
class OemSettingsModel;

// Main window of deepin-installer-oem tool.
class OemWindow : public QFrame {
  Q_OBJECT

 public:
  explicit OemWindow(QWidget* parent = nullptr);

  // Initialize internal data.
  bool init();

 private:
  void initConnections();
  void initUI();

  OemSettingsModel* model_ = nullptr;
  QListView* name_list_view_ = nullptr;
  OemSettingsItemView* item_view_ = nullptr;

 private slots:
  // Update item in item viewer when it is selected.
  void onNameListViewSelected(const QModelIndex& current,
                              const QModelIndex& previous);
};

}  // namespace installer

#endif  // INSTALLER_OEM_OEM_WINDOW_H