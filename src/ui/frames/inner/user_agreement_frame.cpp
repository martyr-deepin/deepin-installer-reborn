#include "user_agreement_frame.h"
#include "base/file_util.h"
#include "service/settings_manager.h"
#include "ui/frames/consts.h"
#include "ui/widgets/nav_button.h"

#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>

using namespace installer;

#ifdef PROFESSIONAL
const QString zh_CN_license{ ":/license/deepin-end-user-license-agreement_zh_CN.txt" };
const QString en_US_license{ ":/license/deepin-end-user-license-agreement_en_US.txt" };
#else
const QString zh_CN_license{ ":/license/deepin-end-user-license-agreement_community_zh_CN.txt" };
const QString en_US_license{ ":/license/deepin-end-user-license-agreement_community_en_US.txt" };
#endif  // PROFESSIONAL

UserAgreementFrame::UserAgreementFrame(QWidget *parent) : QFrame(parent), m_language(QLocale::Language::English), m_type(Type::Chinese)
{
    initUI();
    initConnect();
    updateText();
}

void UserAgreementFrame::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        QLocale locale;
        m_language = locale.language();
        updateText();
    }

    QFrame::changeEvent(event);
}

bool UserAgreementFrame::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_toggleLbl) {
        if (event->type() == QEvent::MouseButtonRelease) {
            toggleLicense();
        }
    }
    return QFrame::eventFilter(watched, event);
}

void UserAgreementFrame::initUI()
{
    m_logoLbl = new QLabel();
    m_logoLbl->setPixmap(QPixmap(installer::GetVendorLogo()));

    m_subTitle = new QLabel(this);
    m_subTitle->setObjectName("user_agreement_subtitle");
    m_sourceLbl = new QLabel(this);
    m_sourceLbl->setObjectName("user_agreement_sourceLbl");
    m_sourceLbl->setWordWrap(true);

    QWidget *sourceWidget = new QWidget;
    sourceWidget->setObjectName("sourceWidget");
    QHBoxLayout *sourceLayout = new QHBoxLayout;
    sourceLayout->setMargin(0);
    sourceLayout->setSpacing(0);
    sourceLayout->addWidget(m_sourceLbl, 0, Qt::AlignTop | Qt::AlignHCenter);
    sourceLayout->addStretch();
    sourceLayout->setContentsMargins(10, 10, 5, 0);
    sourceWidget->setLayout(sourceLayout);

    m_sourceScrollArea = new QScrollArea(this);
    m_sourceScrollArea->setWidget(sourceWidget);
    m_sourceScrollArea->setObjectName("scrollarea");
    m_sourceScrollArea->setWidgetResizable(true);
    m_sourceScrollArea->setFocusPolicy(Qt::NoFocus);
    m_sourceScrollArea->setFrameStyle(QFrame::NoFrame);
    m_sourceScrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_sourceScrollArea->setContentsMargins(0, 0, 0, 0);
    m_sourceScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sourceScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_sourceScrollArea->setContextMenuPolicy(Qt::NoContextMenu);
    m_sourceScrollArea->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    m_sourceScrollArea->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    QScroller::grabGesture(m_sourceScrollArea, QScroller::TouchGesture);

    m_sourceScrollArea->setFixedWidth(468);

    m_toggleLbl = new QLabel;
    m_toggleLbl->installEventFilter(this);

    m_back = new NavButton(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(kMainLayoutSpacing);

    mainLayout->addSpacing(30);
    mainLayout->addWidget(m_logoLbl, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_subTitle, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_sourceScrollArea, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_toggleLbl, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_back, 0, Qt::AlignHCenter);

    setLayout(mainLayout);
    setStyleSheet(installer::ReadFile(":/styles/user_agreement_frame.css"));
}

void UserAgreementFrame::initConnect()
{
    connect(m_back, &NavButton::clicked, this, &UserAgreementFrame::back);
}

void UserAgreementFrame::updateText()
{
    m_subTitle->setText(tr("End User License Agreement"));

    const QString &locale{ installer::ReadLocale() };

    if (locale == "zh_CN") {
        m_toggleLbl->hide();
        m_sourceLbl->setText(installer::ReadFile(zh_CN_license));
    }
    else {
        m_toggleLbl->show();
        m_type = Chinese;
        toggleLicense();
    }

    m_back->setText(tr("Back"));
}

void UserAgreementFrame::toggleLicense()
{
    if (m_type == Chinese) {
        m_sourceLbl->setText(installer::ReadFile(en_US_license));
        m_type = English;
        m_toggleLbl->setText(QString("<u>%1</u>").arg(tr("View in Chinese")));
    }
    else {
        m_sourceLbl->setText(installer::ReadFile(zh_CN_license));
        m_type = Chinese;
        m_toggleLbl->setText(QString("<u>%1</u>").arg(tr("View in English")));
    }
}
