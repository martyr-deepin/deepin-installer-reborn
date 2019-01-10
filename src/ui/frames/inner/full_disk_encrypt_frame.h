#ifndef FULL_DISK_ENCRYPT_FRAME_H
#define FULL_DISK_ENCRYPT_FRAME_H

#include "partman/device.h"

#include <QWidget>

class QVBoxLayout;
class QLabel;
class QCheckBox;

namespace installer {
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

    void setDevice(const Device::Ptr device);

signals:
    void cancel();
    void finished();

protected:
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void onNextBtnClicked();
    void onEncryptUpdated(bool checked);
    void updateText();
    void updateEditCapsLockState(bool on);

private:
    Device *m_device;
    QVBoxLayout *m_layout;
    TitleLabel *m_frameLbl;
    QLabel *m_frameSubLbl;
    QLabel *m_devicePathLbl;
    QLabel *m_deviceModelLbl;
    QLabel *m_deviceSizeLbl;
    QCheckBox *m_encryptCheck;
    QLabel *m_encryptLbl;
    QLabel *m_encryptCheckLbl;
    LineEdit *m_encryptEdit;
    LineEdit *m_encryptRepeatEdit;
    NavButton *m_cancelBtn;
    NavButton *m_nextBtn;
    SystemInfoTip *m_errTip;
    QList<LineEdit*> m_editList;
};
}

#endif // FULL_DISK_ENCRYPT_FRAME_H
