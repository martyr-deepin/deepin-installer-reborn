// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_TOOLTIP_CONTAINER_H
#define INSTALLER_UI_WIDGETS_TOOLTIP_CONTAINER_H

#include <QFrame>
class QPaintEvent;

namespace installer {

// Used to display tooltip with round corners and an arrow at bottom edge.
// The content of tooltip is set with setWidget() method.
class TooltipContainer : public QFrame{
  Q_OBJECT

 public:
  explicit TooltipContainer(QWidget* parent = nullptr);

  // Set content widget of this container.
  // Parent widget of |content| is set to this container.
  void setWidget(QWidget* widget);

  QWidget* widget() const;

 public slots:
  // Show tooltip container at |pos|.
  void popup(const QPoint& pos);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QWidget* child_widget_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_TOOLTIP_CONTAINER_H
