// Copyright (c) 2015 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#include "global_shortcut.h"

#include <QDebug>
#include <QX11Info>
#include <X11/Xlib.h>

#include "keyboard_event_filter.h"

namespace {

KeyboardEventFilter* g_event_filter = nullptr;

// Convert Qt key to native key code
quint32 nativeKeyCode(Qt::Key key) {
  Display* display = QX11Info::display();
  Q_ASSERT(display);

  const char* keyname;
  // Remap multimedia keys.
  switch (key) {
    case Qt::Key_MediaPrevious:
      keyname = "XF86AudioPrev";
      break;
    case Qt::Key_MediaNext:
      keyname = "XF86AudioNext";
      break;
    case Qt::Key_MediaStop:
      keyname = "XF86AudioStop";
      break;
    case Qt::Key_MediaPause:
      keyname = "XF86AudioPause";
      break;
    case Qt::Key_MediaPlay:
      keyname = "XF86AudioPlay";
      break;
    default:
      keyname = QKeySequence(key).toString().toStdString().c_str();
      break;
  }
  return XKeysymToKeycode(display, XStringToKeysym(keyname));
}

// Convert Qt modifiers to native mod.
quint32 nativeModifiers(Qt::KeyboardModifiers modifiers) {
  quint32 native = 0;
  if (modifiers & Qt::ShiftModifier) {
    native |= ShiftMask;
  }
  if (modifiers & Qt::ControlModifier) {
    native |= ControlMask;
  }
  if (modifiers & Qt::AltModifier) {
    native |= Mod1Mask;
  }
  if (modifiers & Qt::MetaModifier) {
    // Super key.
    native |= Mod4Mask;
  }
  return native;
}

// Convert Qt key sequence to native key and native modifier.
void KeySequenceToKeyCode(const QKeySequence& shortcut,
                          quint32* native_key, quint32* native_mod) {
  Qt::KeyboardModifiers mods = Qt::ShiftModifier | Qt::ControlModifier |
                               Qt::AltModifier | Qt::MetaModifier;
  Qt::Key key = shortcut.isEmpty() ?
                Qt::Key(0) :
                Qt::Key((shortcut[0] ^ mods) & shortcut[0]);
  Qt::KeyboardModifiers mod = shortcut.isEmpty() ?
                              Qt::KeyboardModifiers(0) :
                              Qt::KeyboardModifiers(shortcut[0] & mods);
  *native_key = nativeKeyCode(key);
  *native_mod = nativeModifiers(mod);
}

}  // namespace

GlobalShortcut::GlobalShortcut(QKeySequence key_sequence, QObject* parent)
    : QObject(parent),
      is_registered_(false),
      key_(0),
      mod_(0) {
  if (!g_event_filter) {
    g_event_filter = new KeyboardEventFilter();
  }

  KeySequenceToKeyCode(key_sequence, &key_, &mod_);
}

GlobalShortcut::~GlobalShortcut() {
  Q_ASSERT(g_event_filter);

  if (is_registered_) {
    g_event_filter->unregisterShortcut(key_, mod_);
    g_event_filter->shortcuts_.remove(qMakePair(key_, mod_));
  }

  if (g_event_filter->shortcuts_.isEmpty()) {
    delete g_event_filter;
    g_event_filter = nullptr;
  }
}

bool GlobalShortcut::registerNow() {
  is_registered_ = g_event_filter->registerShortcut(key_, mod_);
  if (is_registered_) {
    g_event_filter->shortcuts_.insert(qMakePair(key_, mod_), this);
  }
  return is_registered_;
}