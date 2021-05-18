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

#include "ivmodel.h"

#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QRectF>

class QMutex;
class QGraphicsItem;
class QGraphicsScene;

namespace ivm {
class IVObject;
} // namespace ivm

namespace shared {
class DelayedSignal;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class InterfaceTabGraphicsScene;
class IVCommentGraphicsItem;
class IVInterfaceGraphicsItem;
class IVFunctionGraphicsItem;
class IVFunctionTypeGraphicsItem;
class IVConnectionGraphicsItem;

/*!
   \class IVItemModel is the model represents the scene graph of the currently selected/visible
   IV model. It handles all changes with existing entities and reflects them on graphic scene.
 */

class IVItemModel : public QObject
{
    Q_OBJECT
public:
    enum class Roles
    {
        IdRole = Qt::UserRole,
        ItemVisible,
    };
    Q_ENUM(Roles);

    explicit IVItemModel(ivm::IVModel *model, cmd::CommandsStack *commandsStack, QObject *parent = nullptr);
    ~IVItemModel() override;

    QGraphicsScene *scene() const;

    void changeRootItem(shared::Id id);
    void zoomChanged();

    QGraphicsItem *getItem(const shared::Id id) const;

    ivm::IVModel *objectsModel() const;

Q_SIGNALS:
    void itemClicked(shared::Id id);
    void itemDoubleClicked(shared::Id id);
    void itemsSelected(const QList<shared::Id> &ids);

public Q_SLOTS:
    void updateSceneRect();
    void clearScene();

private Q_SLOTS:
    void onIVObjectAdded(ivm::IVObject *object);
    void onObjectsAdded(const QVector<shared::Id> &objectsIds);
    void onObjectRemoved(shared::Id objectId);
    void onRootObjectChanged(shared::Id rootId);
    void onConnectionAddedToGroup(ivm::IVConnection *connection);
    void onConnectionRemovedFromGroup(ivm::IVConnection *connection);

    void onSceneSelectionChanged();

    void scheduleInterfaceTextUpdate();
    void updateInterfaceTexts();

private:
    QGraphicsItem *createItemForObject(ivm::IVObject *obj);
    void initItem(ivm::IVObject *object, QGraphicsItem *item);
    IVFunctionGraphicsItem *rootItem() const;
    void updateItem(QGraphicsItem *item);
    void removeItemForObject(shared::Id objectId);
    void setupInnerGeometry(ivm::IVObject *obj) const;

    template<typename T>
    T getItem(const shared::Id id) const;

private:
    ivm::IVModel *m_model { nullptr };
    InterfaceTabGraphicsScene *m_graphicsScene { nullptr };
    QHash<shared::Id, QGraphicsItem *> m_items;
    QMutex *m_mutex { nullptr };
    QQueue<shared::Id> m_rmQueu;
    QRectF m_desktopGeometry;
    QRectF m_prevItemsRect;
    shared::DelayedSignal *m_textUpdate = nullptr;
    QPointer<cmd::CommandsStack> m_commandsStack;
};

} // namespace ive
