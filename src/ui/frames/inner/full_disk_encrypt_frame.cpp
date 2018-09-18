#include "full_disk_encrypt_frame.h"

#include "base/file_util.h"
#include "../../../partman/device.h"
#include "../../widgets/rounded_progress_bar.h"
#include "../../widgets/nav_button.h"
#include "../../widgets/line_edit.h"
#include "../../utils/widget_util.h"
#include "../../widgets/title_label.h"
#include "../../../service/settings_manager.h"
#include "../../widgets/system_info_tip.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>

using namespace installer;

const int kProgressBarWidth = 280;

Full_Disk_Encrypt_frame::Full_Disk_Encrypt_frame(QWidget *parent)
    : QWidget(parent)
    , m_device(nullptr)
    , m_layout(new QVBoxLayout(this))
    , m_frameLbl(new TitleLabel(""))
    , m_frameSubLbl(new QLabel)
    , m_encryptCheck(new QCheckBox)
    , m_encryptLbl(new QLabel)
    , m_encryptCheckLbl(new QLabel)
    , m_encryptEdit(new LineEdit(""))
    , m_encryptRepeatEdit(new LineEdit(""))
    , m_cancelBtn(new NavButton)
    , m_nextBtn(new NavButton)
    , m_errTip(new SystemInfoTip(this))
{
    m_layout->setMargin(0);
    m_layout->setSpacing(10);

    m_errTip->hide();

    m_encryptCheck->setCheckable(true);
    m_encryptCheck->setChecked(true);

    setObjectName("FullDiskEncryptFrame");

    m_layout->addStretch();

    // add encrypt label
    m_layout->addWidget(m_frameLbl, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_frameSubLbl, 0, Qt::AlignHCenter);

    // add disk label
    QLabel *diskLbl = new QLabel;
    diskLbl->setPixmap(installer::renderPixmap(":/images/driver_128.svg"));
    m_layout->addWidget(diskLbl, 0, Qt::AlignHCenter);

    // add round progress bar
//    RoundedProgressBar *usageBar = new RoundedProgressBar();
//    usageBar->setFixedSize(kProgressBarWidth, 8);
//    QHBoxLayout *diskLayout = new QHBoxLayout;
//    diskLayout->setMargin(0);
//    diskLayout->setSpacing(0);
//    QLabel *diskPoint = new QLabel;
//    QLabel *diskSize = new QLabel;
//    diskLayout->addWidget(diskPoint, 0, Qt::AlignLeft);
//    diskLayout->addWidget(diskSize, 0, Qt::AlignRight);
//    m_layout->addLayout(diskLayout);
//    m_layout->addWidget(usageBar, 0, Qt::AlignHCenter);

    // add encrypt checkbox
    m_layout->addWidget(m_encryptCheck, 0, Qt::AlignHCenter);

    // add encrypt input
    m_layout->addWidget(m_encryptLbl, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_encryptEdit, 0, Qt::AlignHCenter);
    m_layout->addSpacing(10);
    m_layout->addWidget(m_encryptCheckLbl, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_encryptRepeatEdit, 0, Qt::AlignHCenter);

    m_layout->addStretch();

    // add buttons
    m_layout->addWidget(m_cancelBtn, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_nextBtn, 0, Qt::AlignHCenter);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(m_layout);

    connect(m_cancelBtn, &NavButton::clicked, this, &Full_Disk_Encrypt_frame::cancel);
    connect(m_nextBtn, &NavButton::clicked, this, &Full_Disk_Encrypt_frame::onNextBtnClicked);
    connect(m_encryptCheck, &QCheckBox::clicked, this, &Full_Disk_Encrypt_frame::onEncryptUpdated);
    connect(m_encryptEdit, &LineEdit::textChanged, m_errTip, &SystemInfoTip::hide);
    connect(m_encryptRepeatEdit, &LineEdit::textChanged, m_errTip, &SystemInfoTip::hide);

    onEncryptUpdated(true);

    setStyleSheet(ReadFile(":/styles/full_encrypt_frame.css"));
}

bool Full_Disk_Encrypt_frame::isEncrypt() const
{
    return m_encryptCheck->isChecked();
}

void Full_Disk_Encrypt_frame::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        m_frameLbl->setText(tr("Full Disk Encrypt"));
        m_frameSubLbl->setText(tr(""));
        m_encryptCheck->setText(tr("Encrypt This Disk"));
        m_encryptLbl->setText(tr("Encrypt Password"));
        m_encryptCheckLbl->setText(tr("Repeat Encrypt Password"));
        m_cancelBtn->setText(tr("Cancel"));
        m_nextBtn->setText(tr("Next"));
    }

    return QWidget::changeEvent(event);
}

void Full_Disk_Encrypt_frame::onNextBtnClicked()
{
    if (m_encryptCheck->isChecked()) {
        // check password
        if (m_encryptEdit->text() != m_encryptRepeatEdit->text()) {
            m_errTip->setText(tr("两次密码不一致"));
            m_errTip->showBottom(m_encryptRepeatEdit);
            return;
        }

        WriteFullDiskEncryptPassword(m_encryptEdit->text());
    }
    else {
        WriteFullDiskEncryptPassword("");
    }

    emit finished();
}

void Full_Disk_Encrypt_frame::onEncryptUpdated(bool checked)
{
    m_encryptCheckLbl->setEnabled(checked);
    m_encryptEdit->setEnabled(checked);
    m_encryptLbl->setEnabled(checked);
    m_encryptRepeatEdit->setEnabled(checked);
}
