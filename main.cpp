// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "application.h"
#include "ui/main_window.h"

Application* app;

int main(int argc, char* argv[]) {
  app = new Application(argc, argv);

  app->initServiceObjects();
  // TODO(xushaohua): Do some filter job here.

  app->initMainWindow();
  app->main_window->showFullScreen();

  const int state = app->exec();
  delete app;
  return state;
}