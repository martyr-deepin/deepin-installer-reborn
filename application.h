// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_APPLICATION_H
#define DEEPIN_INSTALLER_APPLICATION_H

#include <QApplication>

namespace service {
class SignalManager;
}  // namespace service

class Application : public QApplication {
  Q_OBJECT

 public:
  Application(int& argc, char** argv) : QApplication(argc, argv) { }
  // Destroy service objects in destructor.
  ~Application();

  service::SignalManager* signal_manager = nullptr;

  void initServiceObjects();
};

// Global instance.
extern Application* app;

#endif  // DEEPIN_INSTALLER_APPLICATION_H
