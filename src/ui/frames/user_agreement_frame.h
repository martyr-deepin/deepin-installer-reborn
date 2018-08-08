#ifndef INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H
#define INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H

#include <QFrame>
#include <QVBoxLayout>

class QLabel;
class QPushButton;
class QScrollArea;

namespace installer {
class NavButton;
class UserAgreementFrame : public QFrame
{
    Q_OBJECT
public:
    explicit UserAgreementFrame(QWidget *parent = nullptr);

signals:
    // Emitted when back button is clicked.
    void finished();

    // Emitted when cancel
    void cancel();

public slots:

protected:
    // Update text of next_button_
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    void updateText();

private:
    QLabel *m_logoLbl;
    QLabel *m_subTitle;
    QLabel *m_sourceLbl;
    NavButton *m_accept;
    NavButton *m_cancel;
    QScrollArea *m_sourceScrollArea;
};
}
#endif // INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H
