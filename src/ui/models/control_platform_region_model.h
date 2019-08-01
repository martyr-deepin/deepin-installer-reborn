#ifndef CONTROL_PLATFORM_REGION_MODEL
#define CONTROL_PLATFORM_REGION_MODEL

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <memory>

namespace installer {
class RegionInfo {
public:
    typedef std::shared_ptr<RegionInfo> Ptr;
    int     Id { 0 };
    QString level_struct;
    QString name;
    QString des;
    int     user_count { 0 };
    int     pc_count { 0 };
    int     software_count { 0 };
    int     parent_id { 0 };
    QList<RegionInfo::Ptr> children;

    void formJson(const QJsonObject& obj)
    {
        Id             = obj["id"].toInt();
        level_struct   = obj["level_struct"].toString();
        name           = obj["name"].toString();
        des            = obj["des"].toString();
        user_count     = obj["user_count"].toInt();
        pc_count       = obj["pc_count"].toInt();
        software_count = obj["software_count"].toInt();
        parent_id      = obj["parent_id"].toInt();
    }

    const QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["id"]             = Id;
        obj["level_struct"]   = level_struct;
        obj["name"]           = name;
        obj["des"]            = des;
        obj["user_count"]     = user_count;
        obj["pc_count"]       = pc_count;
        obj["software_count"] = software_count;
        obj["parent_id"]      = parent_id;
        return obj;
    }
};

class ControlPlatformRegionModel : public QAbstractListModel {
    Q_OBJECT
public:
    ControlPlatformRegionModel(QObject* parent = nullptr);
    void setList(const QList<RegionInfo::Ptr>& list);
    RegionInfo::Ptr findInfoByIndex(int index) const;

    QVariant data(const QModelIndex& index, int role) const override;
    int      rowCount(const QModelIndex& parent) const override;

private:
    QList<RegionInfo::Ptr> m_list;
};
}  // namespace installer

QDebug& operator<<(QDebug& debug, const installer::RegionInfo& info);
QDebug& operator<<(QDebug& debug, const installer::RegionInfo::Ptr info);

#endif  // !CONTROL_PLATFORM_REGION_MODEL
