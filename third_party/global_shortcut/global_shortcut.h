// Copyright (c) 2015 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#ifndef INSTALLER_THIRD_PARTY_GLOBAL_SHORTCUT_H_
#define INSTALLER_THIRD_PARTY_GLOBAL_SHORTCUT_H_

#include <QObject>
#include <QKeySequence>

// GlobalShortcut is used to grab global keyboard press event.
// It shall be used only in main thread(UI thread).
class GlobalShortcut : public QObject {
  Q_OBJECT
 public:
  explicit GlobalShortcut(QKeySequence key_sequence, QObject* parent = nullptr);
  ~GlobalShortcut();

  bool isRegistered() const { return is_registered_; }

  // Register this shortcut.
  // Returns true is success or false otherwise.
  bool registerNow();

 signals:
  // Emitted when current key sequence is pressed.
  void activated();

 private:
  bool is_registered_;
  quint32 key_;
  quint32 mod_;
};

#endif  // INSTALLER_THIRD_PARTY_GLOBAL_SHORTCUT_H_
