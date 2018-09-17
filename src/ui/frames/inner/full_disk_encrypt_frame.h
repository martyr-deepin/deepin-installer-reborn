#ifndef FULL_DISK_ENCRYPT_FRAME_H
#define FULL_DISK_ENCRYPT_FRAME_H

#include <QWidget>

class QVBoxLayout;
class QLabel;
class QCheckBox;

namespace installer {
class Device;
class RoundedProgressBar;
class NavButton;
class LineEdit;
class TitleLabel;
class SystemInfoTip;
class Full_Disk_Encrypt_frame : public QWidget
{
    Q_OBJECT
public:
    explicit Full_Disk_Encrypt_frame(QWidget *parent = nullptr);

signals:
    void cancel();
    void finished();

protected:
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void onNextBtnClicked();
    void onEncryptUpdated(bool checked);

private:
    Device *m_device;
    QVBoxLayout *m_layout;
    TitleLabel *m_frameLbl;
    QLabel *m_frameSubLbl;
    QCheckBox *m_encryptCheck;
    QLabel *m_encryptLbl;
    QLabel *m_encryptCheckLbl;
    LineEdit *m_encryptEdit;
    LineEdit *m_encryptRepeatEdit;
    NavButton *m_cancelBtn;
    NavButton *m_nextBtn;
    SystemInfoTip *m_errTip;
};
}

#endif // FULL_DISK_ENCRYPT_FRAME_H
