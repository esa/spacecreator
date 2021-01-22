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

namespace aadl {
class AADLObject;
class AADLObjectsModel;
class AADLObjectConnection;
}

namespace ive {

class CommonVisualizationModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CommonVisualizationModel(aadl::AADLObjectsModel *aadlModel, QObject *parent = nullptr);
    enum ItemRole
    {
        IdRole = Qt::UserRole + 1,
        TypeRole
    };

    QStandardItem *getItem(const shared::Id id);

protected:
    virtual void updateItemData(QStandardItem *item, aadl::AADLObject *obj);
    virtual QStandardItem *createItem(aadl::AADLObject *obj);
    QStandardItem *getParentItem(aadl::AADLObject *obj);
    QStandardItem *getItem(aadl::AADLObject *obj);
    void addItem(aadl::AADLObject *obj);

private Q_SLOTS:
    void addItems(const QVector<aadl::AADLObject *> &objects);
    void removeItem(aadl::AADLObject *object);
    void updateItem();
    void updateConnectionItem(aadl::AADLObjectConnection *connection);

protected:
    aadl::AADLObjectsModel *m_aadlModel { nullptr };
    QHash<shared::Id, QStandardItem *> m_itemCache;
};

class VisualizationModel : public CommonVisualizationModel
{
    Q_OBJECT
public:
    explicit VisualizationModel(aadl::AADLObjectsModel *aadlModel, QObject *parent = nullptr);

    void updateItemData(QStandardItem *item, aadl::AADLObject *obj) override;
    QStandardItem *createItem(aadl::AADLObject *obj) override;

private Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};

} // namespace ive
