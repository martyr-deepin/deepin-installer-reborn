#include "user_agreement_frame.h"
#include "service/settings_manager.h"
#include "base/file_util.h"
#include "ui/widgets/nav_button.h"
#include "ui/frames/consts.h"

#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>

using namespace installer;

UserAgreementFrame::UserAgreementFrame(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
    updateText();
}

void UserAgreementFrame::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        updateText();
    }

    QFrame::changeEvent(event);
}

void UserAgreementFrame::initUI()
{
    m_logoLbl = new QLabel();
    m_logoLbl->setPixmap(QPixmap(installer::GetVendorLogo()));

    m_subTitle  = new QLabel(this);
    m_subTitle->setObjectName("user_agreement_subtitle");
    m_sourceLbl = new QLabel(this);
    m_sourceLbl->setObjectName("user_agreement_sourceLbl");
    m_sourceLbl->setFixedWidth(468);
    m_sourceLbl->setWordWrap(true);
    m_sourceLbl->setContentsMargins(10, 10, 10,10);

    QWidget *sourceWidget = new QWidget;
    sourceWidget->setObjectName("sourceWidget");
    QHBoxLayout *sourceLayout = new QHBoxLayout;
    sourceLayout->setMargin(0);
    sourceLayout->setSpacing(0);
    sourceLayout->addWidget(m_sourceLbl, 0, Qt::AlignTop);
    sourceLayout->addStretch();
    sourceWidget->setLayout(sourceLayout);

    m_sourceScrollArea = new QScrollArea(this);
    m_sourceScrollArea->setWidget(sourceWidget);
    m_sourceScrollArea->setObjectName("scrollarea");
    m_sourceScrollArea->setWidgetResizable(true);
    m_sourceScrollArea->setFocusPolicy(Qt::NoFocus);
    m_sourceScrollArea->setFrameStyle(QFrame::NoFrame);
    m_sourceScrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_sourceScrollArea->setContentsMargins(0, 0, 10, 0);
    m_sourceScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sourceScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_accept = new NavButton(this);
    m_cancel = new NavButton(this);

    m_accept->setDisabled(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(kMainLayoutSpacing);

    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_logoLbl, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_subTitle, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_sourceScrollArea, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_accept, 0 , Qt::AlignHCenter);
    mainLayout->addWidget(m_cancel, 0 , Qt::AlignHCenter);

    setLayout(mainLayout);
    setStyleSheet(installer::ReadFile(":/styles/user_agreement_frame.css"));
}

void UserAgreementFrame::initConnect()
{
    connect(m_accept, &NavButton::clicked, this, &UserAgreementFrame::finished);
    connect(m_cancel, &NavButton::clicked, this, &UserAgreementFrame::cancel);
    connect(m_sourceScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, [=] (int value) {
        if (!m_accept->isEnabled() && value == m_sourceScrollArea->verticalScrollBar()->maximum()) {
            m_accept->setDisabled(false);
        }
    });
}

void UserAgreementFrame::updateText()
{
    m_subTitle->setText(tr("End-user license agreement"));

    const QString &locale { installer::ReadLocale() };

    if (locale == "zh_CN") {
        m_sourceLbl->setText(
                    installer::ReadFile(
                        QString(":/license/deepin-end-user-license-agreement_zh_CN.txt")
                        .arg(locale)));
    }
    else {
        m_sourceLbl->setText(
                    installer::ReadFile(
                        QString(":/license/deepin-end-user-license-agreement_en_US.txt")));
    }

    m_accept->setText(tr("Accept"));
    m_cancel->setText(tr("Cancel"));
}
