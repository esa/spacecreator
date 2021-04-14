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

#include <QPointer>
#include <QStandardItemModel>

namespace ivm {
class IVObject;
class IVModel;
class IVConnection;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class CommonVisualizationModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CommonVisualizationModel(
            ivm::IVModel *aadlModel, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);
    enum ItemRole
    {
        IdRole = Qt::UserRole + 1,
        TypeRole
    };

    QStandardItem *getItem(const shared::Id id);

protected:
    virtual void updateItemData(QStandardItem *item, ivm::IVObject *obj);
    virtual QStandardItem *createItem(ivm::IVObject *obj);
    QStandardItem *getParentItem(ivm::IVObject *obj);
    QStandardItem *getItem(ivm::IVObject *obj);
    void addItem(ivm::IVObject *obj);

private Q_SLOTS:
    void addItems(const QVector<ivm::IVObject *> &objects);
    void removeItem(ivm::IVObject *object);
    void updateItem();
    void updateConnectionItem(ivm::IVConnection *connection);

protected:
    ivm::IVModel *m_aadlModel { nullptr };
    QHash<shared::Id, QStandardItem *> m_itemCache;
    QPointer<cmd::CommandsStack> m_commandsStack;
};

class VisualizationModel : public CommonVisualizationModel
{
    Q_OBJECT
public:
    explicit VisualizationModel(
            ivm::IVModel *aadlModel, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);

    void updateItemData(QStandardItem *item, ivm::IVObject *obj) override;
    QStandardItem *createItem(ivm::IVObject *obj) override;

private Q_SLOTS:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};

} // namespace ive
