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

#include "ui/frames/inner/prepare_install_frame.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PrepareInstallFrame::PrepareInstallFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("prepare_install_frame");

  this->initUI();
  this->initConnections();
}

void PrepareInstallFrame::updateDescription(const QStringList& descriptions) {
  const QString prefix("•   ");
  QStringList modified_desc_list;
  int max_width = 0;
  QFontMetrics metrics(description_edit_->font());
  for (const QString& description : descriptions) {
    const QString content = prefix + description;
    modified_desc_list.append(content);
    max_width = qMax(metrics.width(content), max_width);
  }
  const QString description_text = modified_desc_list.join("\n");
  qDebug() << "description:" << description_text;
  description_edit_->setPlainText(description_text);
  Q_ASSERT(max_width >= 0);
  // Add horizontal margins.
  description_edit_->setFixedWidth(max_width + 20);
}

void PrepareInstallFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Prepare for Installation"));
    comment_label_->setText(
        tr("Make a backup of your important data and then continue"));
    subtitle_label_->setText(
        tr("The following operations will be executed, "
           "please confirm and continue to avoid data loss"));
    abort_button_->setText(tr("Back"));
    continue_button_->setText(tr("Continue"));
  } else {
    QFrame::changeEvent(event);
  }
}

void PrepareInstallFrame::initConnections() {
  connect(abort_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::aborted);
  connect(continue_button_, &QPushButton::clicked,
          this, &PrepareInstallFrame::finished);
}

void PrepareInstallFrame::initUI() {
  title_label_ = new TitleLabel(tr("Prepare for Installation"));
  comment_label_ = new CommentLabel(
      tr("Make a backup of your important data and then continue"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  subtitle_label_ = new QLabel(
      tr("The following operations will be executed, "
         "please confirm and continue to avoid data loss"));
  subtitle_label_->setObjectName("subtitle_label");

  description_edit_ = new QTextEdit();
  description_edit_->setObjectName("description_edit");
  description_edit_->setContentsMargins(10, 10, 10, 10);
  description_edit_->setAcceptRichText(false);
  description_edit_->setReadOnly(true);
  description_edit_->setContextMenuPolicy(Qt::NoContextMenu);
  description_edit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  description_edit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  abort_button_ = new NavButton(tr("Back"));
  continue_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(subtitle_label_, 0, Qt::AlignCenter);
  layout->addWidget(description_edit_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);
  layout->addSpacing(kNavButtonVerticalSpacing);
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/prepare_install_frame.css"));
}

}  // namespace installer
