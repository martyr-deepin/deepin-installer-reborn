#include <QComboBox>
#include <QVBoxLayout>

#include "control_platform_frame.h"
#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"

using namespace installer;

ControlPlatformFrame::ControlPlatformFrame(QWidget *parent)
    : QWidget(parent)
    , m_serverLineEdit(new LineEdit(QString(":/images/hostname_12.svg")))
    , m_regionBox(new QComboBox)
    , m_nextButton(new NavButton)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);

    layout->addStretch();
    layout->addWidget(m_serverLineEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_regionBox, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(m_nextButton, 0, Qt::AlignHCenter);

    setLayout(layout);
}
