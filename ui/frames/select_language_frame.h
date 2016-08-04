// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_SELECT_LANGUAGE_FRAME_H

#include <QFrame>
class QListView;

namespace ui {

class LanguageListModel;
class NavButton;

// Select system language.
class SelectLanguageFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectLanguageFrame(QWidget* parent = nullptr);

  void autoConf();

 signals:
  // Emitted when system language is selected.
  void finished();

 private:
  void initConnections();
  void initUI();

  bool language_is_selected_ = false;

  QListView* list_view_ = nullptr;
  LanguageListModel* list_model_ = nullptr;

  NavButton* next_button_ = nullptr;

 private slots:
  void onLanguageListSelected();

  // Check if a language item is selected.
  void onNextButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
