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

#include <QGraphicsItem>
#include <QPointer>
#include <QQueue>
#include <QRectF>
#include <memory>

class QGraphicsScene;
class QGraphicsItem;
class QMutex;

namespace shared {
class VEModel;
namespace cmd {
class CommandsStackBase;
}

namespace ui {
class VEInteractiveObject;

class VEItemModel : public QObject
{
    Q_OBJECT
public:
    explicit VEItemModel(VEModel *model, cmd::CommandsStackBase *commandsStack, QObject *parent = nullptr);
    ~VEItemModel() override;

    QGraphicsScene *scene() const;
    QGraphicsItem *getItem(const shared::Id id) const;
    virtual VEModel *objectsModel() const;

    void zoomChanged();
    void updateSceneRect();
    void clearScene();

Q_SIGNALS:
    void itemClicked(shared::Id id);
    void itemDoubleClicked(shared::Id id);
    void itemsSelected(const QList<shared::Id> &ids);

private:
    VEInteractiveObject *createItemForObject(shared::Id objectId);
    void updateItem(VEInteractiveObject *item);
    void removeItemForObject(shared::Id objectId);

    void onObjectsAdded(const QVector<shared::Id> &objectsIds);
    void onSceneSelectionChanged();

protected:
    template<typename T>
    T getItem(const shared::Id id) const
    {
        if (id.isNull()) {
            return nullptr;
        }

        return qgraphicsitem_cast<T>(m_items.value(id));
    }

    virtual VEInteractiveObject *createItem(shared::Id objectId) = 0;
    virtual void initItem(VEInteractiveObject *item);

    virtual void onObjectAdded(shared::Id objectId);
    virtual void onObjectRemoved(shared::Id objectId);
    virtual void onObjectAttributeChanged();

protected:
    QPointer<VEModel> m_model;
    QPointer<cmd::CommandsStackBase> m_commandsStack;

    QGraphicsScene *m_graphicsScene { nullptr };
    std::unique_ptr<QMutex> m_mutex;
    QHash<shared::Id, VEInteractiveObject *> m_items;
    QQueue<shared::Id> m_rmQueu;
    QRectF m_desktopGeometry;
    QRectF m_prevItemsRect;
};

} // namespace ui
} // namespace shared
