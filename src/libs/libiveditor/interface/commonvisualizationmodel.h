/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "common.h"

#include <QStandardItemModel>

namespace ivm {
class AADLObject;
class AADLModel;
class AADLConnection;
}

namespace ive {

class CommonVisualizationModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CommonVisualizationModel(ivm::AADLModel *aadlModel, QObject *parent = nullptr);
    enum ItemRole
    {
        IdRole = Qt::UserRole + 1,
        TypeRole
    };

    QStandardItem *getItem(const shared::Id id);

protected:
    virtual void updateItemData(QStandardItem *item, ivm::AADLObject *obj);
    virtual QStandardItem *createItem(ivm::AADLObject *obj);
    QStandardItem *getParentItem(ivm::AADLObject *obj);
    QStandardItem *getItem(ivm::AADLObject *obj);
    void addItem(ivm::AADLObject *obj);

private Q_SLOTS:
    void addItems(const QVector<ivm::AADLObject *> &objects);
    void removeItem(ivm::AADLObject *object);
    void updateItem();
    void updateConnectionItem(ivm::AADLConnection *connection);

protected:
    ivm::AADLModel *m_aadlModel { nullptr };
    QHash<shared::Id, QStandardItem *> m_itemCache;
};

class VisualizationModel : public CommonVisualizationModel
{
    Q_OBJECT
public:
    explicit VisualizationModel(ivm::AADLModel *aadlModel, QObject *parent = nullptr);

    void updateItemData(QStandardItem *item, ivm::AADLObject *obj) override;
    QStandardItem *createItem(ivm::AADLObject *obj) override;

private Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};

} // namespace ive
