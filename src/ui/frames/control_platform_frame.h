#ifndef CONTROLPLATFORMFRAME_H
#define CONTROLPLATFORMFRAME_H

#include <QUrl>
#include <QWidget>

#include "ui/models/control_platform_region_model.h"

class QComboBox;
class QNetworkReply;
class QVBoxLayout;
namespace installer {
class LineEdit;
class NavButton;
class ControlPlatformRegionModel;
class TableComboBox;
class TitleLabel;
class CommentLabel;
class ControlPlatformFrame : public QWidget {
    Q_OBJECT
public:
    ControlPlatformFrame(QWidget* parent = nullptr);

signals:
    void requestFinished();

protected:
    bool event(QEvent* event) override;

private:
    void onNetworkFinished(QNetworkReply* reply);
    void onNextClicked();
    void onRegionSelected();

private slots:
    void onNetworkStateChanged();
    TableComboBox* createComboBox(const QList<RegionInfo::Ptr>& list);

private:
    TitleLabel*                 m_titleLbl;
    CommentLabel*               m_subTitleLbl;
    LineEdit*                   m_serverLineEdit;
    NavButton*                  m_nextButton;
    QUrl                        m_serverUrl;
    RegionInfo::Ptr             m_regionInfo;
    QVBoxLayout*                m_macInfoLayout;
    QVBoxLayout*                m_ipInfoLayout;
    QVBoxLayout*                m_regionBoxLayout;
    QList<QList<RegionInfo::Ptr>> m_regionInfoList;
    QList<TableComboBox*>       m_tableComboBoxList;
};
}  // namespace installer
#endif  // !CONTROLPLATFORMFRAME_H
