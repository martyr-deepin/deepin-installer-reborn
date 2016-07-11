// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_UI_MAIN_WINDOW_H
#define DEEPIN_INSTALLER_UI_MAIN_WINDOW_H

#include <QWidget>
class QLabel;
class QResizeEvent;

namespace ui {

class IconButton;

// MainWindow is a fullscreen window.
// All of ui frames are placed in MainWindow.
// It does following jobs:
//   * draw window background with a blur image;
//   * handles keyboard shortcut like Ctrl+P and Ctrl+L;
//   * handles window navigation (by providing a content area);
//   * quit / abort installation process.
class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow();

  // Show/hide close button.
  void setCloseButtonVisible(bool visible);
  bool isCloseButtonVisible() const;

 protected:
  // Move close button to appropriate position when window is resized.
  void resizeEvent(QResizeEvent* event) override;

 private:
  void initConnections();
  void initUI();
  void registerShortcut();
  void updateBackground();

  QLabel* background_label_ = nullptr;
  IconButton* close_button_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_UI_MAIN_WINDOW_H
