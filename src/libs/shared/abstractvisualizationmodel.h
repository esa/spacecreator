/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

namespace shared {
class VEModel;
class VEObject;

namespace cmd {
class CommandsStackBase;
}

class AbstractVisualizationModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit AbstractVisualizationModel(
            shared::VEModel *veModel, cmd::CommandsStackBase *commandsStack, QObject *parent = nullptr);

    enum ItemRole
    {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        CursorPixmapRole,
        DropRole,
    };

    QStandardItem *getItem(const shared::Id id);

protected:
    virtual void updateItemData(QStandardItem *item, VEObject *obj);
    virtual QStandardItem *createItem(VEObject *obj);
    QStandardItem *getParentItem(VEObject *obj);
    QStandardItem *getItem(VEObject *obj);

protected Q_SLOTS:
    void updateItem();

private Q_SLOTS:
    void addItem(VEObject *obj);
    void addItems(const QVector<shared::Id> &objectsIds);
    void removeItem(shared::Id objId);

protected:
    QPointer<VEModel> m_veModel;
    QPointer<cmd::CommandsStackBase> m_commandsStack;
    QHash<shared::Id, QStandardItem *> m_itemCache;
};

} // namespace shared
