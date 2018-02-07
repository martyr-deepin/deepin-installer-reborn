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

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_KEYBOARD_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_KEYBOARD_FRAME_H

#include <QFrame>
class QLineEdit;

namespace installer {

class FramelessListView;
class KeyboardLayoutModel;
class KeyboardLayoutVariantModel;
class NavButton;
class TitleLabel;

// Keyboard layout setup page.
class SystemInfoKeyboardFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoKeyboardFrame(QWidget* parent = nullptr);

  // Set keyboard layout to default value.
  void readConf();

 signals:
  // Emitted when back_button_ is clicked.
  void finished();

  // Emitted when new keyboard layout is selected.
  void layoutUpdated(const QString& layout);

 public slots:
  // Save current keyboard layout to settings file.
  void writeConf();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  FramelessListView* layout_view_ = nullptr;
  KeyboardLayoutModel* layout_model_ = nullptr;
  FramelessListView* variant_view_ = nullptr;
  KeyboardLayoutVariantModel* variant_model_ = nullptr;
  QLineEdit* test_edit_ = nullptr;
  NavButton* back_button_ = nullptr;
  QString current_locale_;

 private slots:
  // Update variant list when new keyboard layout is selected.
  // Update system keyboard layout.
  // Emit layoutUpdate() signal.
  // Clear content of test_edit_.
  void onLayoutViewSelectionChanged(const QModelIndex& current,
                                    const QModelIndex& previous);

  // Update system keyboard layout when new layout variant is selected.
  // Clear content of test_edit_.
  void onVariantViewSelected(const QModelIndex& current,
                             const QModelIndex& previous);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_KEYBOARD_FRAME_H
