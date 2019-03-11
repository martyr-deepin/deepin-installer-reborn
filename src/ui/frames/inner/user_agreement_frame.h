#ifndef INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H
#define INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLocale>

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

    enum Type {
        Chinese,
        English,
    };

signals:
    // Emitted when cancel
    void back();

protected:
    // Update text of next_button_
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initUI();
    void initConnect();
    void updateText();
    void toggleLicense();

private:
    QLabel *m_logoLbl;
    QLabel *m_subTitle;
    QLabel *m_sourceLbl;
    QLabel *m_toggleLbl;
    NavButton *m_back;
    QScrollArea *m_sourceScrollArea;
    QLocale::Language m_language;
    Type m_type;
};
}
#endif // INSTALLER_UI_FRAMES_USER_AGREEMENT_FRAME_H
