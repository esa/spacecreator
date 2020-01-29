#pragma once

#include "app/context/action/action.h"

#include <QStringListModel>

namespace taste3 {
namespace aadl {

class ActionsModel : public QStringListModel
{
    Q_OBJECT

public:
    explicit ActionsModel(QObject *parent = nullptr);
    void setupData(QVector<ctx::Action> *collection);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QModelIndex createAction(const QString &title, const QString &target);

private:
    QVector<ctx::Action> *m_collection { nullptr };
};

} // namespace aadl
} // namespace taste3
