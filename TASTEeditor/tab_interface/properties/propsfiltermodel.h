#pragma once

#include "propertieslistmodel.h"

#include <QSortFilterProxyModel>

namespace taste3 {
namespace aadl {

class PropsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PropsFilterModel(QObject *parent = nullptr);
    PropertiesListModel::ItemType targetType() const;
    void setTargetType(PropertiesListModel::ItemType type);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    PropertiesListModel::ItemType m_targetType;
};

} // ns aadl
} // ns taste3
