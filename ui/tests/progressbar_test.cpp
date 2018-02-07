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

#include <QApplication>
#include <QFrame>
#include <QSlider>
#include <QVBoxLayout>

#include "ui/widgets/rounded_progress_bar.h"

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

  QProgressBar* bar = new installer::RoundedProgressBar();
  bar->setFixedSize(480, 10);
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