// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
#define INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H

#include <QFrame>
#include <QModelIndex>
class QTranslator;

#include "service/system_language.h"

namespace installer {

class FramelessListView;
class LanguageListModel;
class NavButton;

// Select system language and update ui language on-the-fly.
class SelectLanguageFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectLanguageFrame(QWidget* parent = nullptr);

  void autoConf();

 signals:
  void finished();

  // Emitted when new language is selected.
  void languageUpdated(const QString& locale);

 protected:
  // Update text of next_button_
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();
  void updateTranslator(const QString& locale);

  // Current selected language.
  LanguageItem lang_;

  QTranslator* current_translator_ = nullptr;

  FramelessListView* language_view_ = nullptr;
  LanguageListModel* language_model_ = nullptr;

  NavButton* next_button_ = nullptr;

 private slots:
  void onLanguageListSelected(const QModelIndex& current,
                              const QModelIndex& previous);

  // Check if a language item is selected.
  void onNextButtonClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_SELECT_LANGUAGE_FRAME_H
