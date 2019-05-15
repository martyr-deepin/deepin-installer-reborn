#ifndef CONTROL_PLATFORM_REGION_MODEL
#define CONTROL_PLATFORM_REGION_MODEL

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

namespace installer {
struct RegionInfo {
    int     Id;
    QString level_struct;
    QString name;
    QString des;
    int     user_count;
    int     pc_count;
    int     software_count;

    void formJson(const QJsonObject& obj)
    {
        Id             = obj["id"].toInt();
        level_struct   = obj["level_struct"].toString();
        name           = obj["name"].toString();
        des            = obj["des"].toString();
        user_count     = obj["user_count"].toInt();
        pc_count       = obj["pc_count"].toInt();
        software_count = obj["software_count"].toInt();
    }

    QJsonObject toJson()
    {
        QJsonObject obj;
        obj["id"]             = Id;
        obj["level_struct"]   = level_struct;
        obj["name"]           = name;
        obj["des"]            = des;
        obj["user_count"]     = user_count;
        obj["pc_count"]       = pc_count;
        obj["software_count"] = software_count;
        return obj;
    }
};

class ControlPlatformRegionModel : public QAbstractListModel {
    Q_OBJECT
public:
    ControlPlatformRegionModel(QObject* parent = nullptr);
    void setList(const QList<RegionInfo>& list);
    const RegionInfo findInfoByIndex(int index) const;

    QVariant data(const QModelIndex& index, int role) const override;
    int      rowCount(const QModelIndex& parent) const override;

private:
    QList<RegionInfo> m_list;
};
}  // namespace installer
#endif  // !CONTROL_PLATFORM_REGION_MODEL
