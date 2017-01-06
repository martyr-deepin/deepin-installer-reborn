// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_OEM_WINDOW_H
#define INSTALLER_UI_OEM_OEM_WINDOW_H

#include <QFrame>
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QScrollArea;
class QSlider;
class QStringListModel;

namespace installer {

class LanguageListModel;
class SettingsModel;

// Main window of deepin-installer-oem tool.
class OemWindow : public QFrame {
  Q_OBJECT

 public:
  explicit OemWindow(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  // Language selection
  QComboBox* default_locale_combo_ = nullptr;
  LanguageListModel* language_model_ = nullptr;

  // Partition
  QComboBox* default_fs_combo_ = nullptr;

  QListView* category_view_ = nullptr;
  QStringListModel* category_model_ = nullptr;
  QScrollArea* right_scroll_area_ = nullptr;

  SettingsModel* settings_model_ = nullptr;

 private slots:
  // Update tooltip of default_locale_combo_ when another language is selected.
  void onDefaultLocaleIndexChanged(int index);
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_OEM_WINDOW_H