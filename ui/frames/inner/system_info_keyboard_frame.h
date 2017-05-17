// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
