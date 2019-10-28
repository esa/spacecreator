#pragma once

#include <QStandardItemModel>

namespace taste3 {
namespace aadl {

class AADLObject;
class PropertiesListModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit PropertiesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool createProperty(const QString& propName);
    bool removeProperty(const QModelIndex& index);

    void setDataObject(AADLObject* obj);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool isAttr(const QModelIndex& id) const;
    bool isProp(const QModelIndex& id) const;

private:
    AADLObject* m_dataObject {nullptr};
    QVector<QString> m_names;

    enum ItemType
    {
        Attribute = 0,
        Property
    };

    void createNewRow( const QString& title, const QVariant& value, ItemType type, int row);
};

} // namespace aadl
} // namespace taste3
