// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QFrame>
#include <QProgressBar>
#include <QSlider>
#include <QVBoxLayout>

namespace {

const char kStyle[] = ""
    "QProgressBar {"
    "  border-radius: 5px;"
    "  color: white;"
    "  background-color: black;"
    "}"
    "QProgressBar::chunk {"
    "  background-color: red;"
    "  border-radius: 5px;"
    "}";

}  // namespace

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QProgressBar* bar = new QProgressBar();
  bar->setFixedSize(480, 20);
  bar->setStyleSheet(kStyle);
  bar->setTextVisible(false);
  bar->setRange(0, 100);
  bar->show();

  QSlider* slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, 100);

  QObject::connect(slider, &QSlider::valueChanged,
                   bar, &QProgressBar::setValue);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addStretch();
  layout->addWidget(bar);
  layout->addWidget(slider);
  layout->addStretch();

  QFrame frame;
  frame.setLayout(layout);
  frame.resize(480, 200);
  frame.show();

  return app.exec();
}