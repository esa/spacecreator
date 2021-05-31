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

#include "veitemmodel.h"

#include "commandsstackbase.h"
#include "graphicsviewutils.h"
#include "ui/veinteractiveobject.h"
#include "vemodel.h"
#include "veobject.h"

#include <QGraphicsScene>
#include <QGuiApplication>
#include <QMutex>
#include <QScreen>

namespace shared {
namespace ui {

VEItemModel::VEItemModel(VEModel *model, cmd::CommandsStackBase *commandsStack, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_commandsStack(commandsStack)
    , m_graphicsScene(new QGraphicsScene(this))
    , m_mutex(new QMutex(QMutex::NonRecursive))
{
    Q_ASSERT(!m_commandsStack.isNull());

    if (QGuiApplication::primaryScreen()) {
        m_desktopGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    }

    connect(m_model, &VEModel::modelReset, this, &VEItemModel::clearScene);
    connect(m_model, &VEModel::objectsAdded, this, &VEItemModel::onObjectsAdded);
    connect(m_model, &VEModel::objectRemoved, this, &VEItemModel::onObjectRemoved);

    connect(m_graphicsScene, &QGraphicsScene::selectionChanged, this, &VEItemModel::onSceneSelectionChanged);
}

VEItemModel::~VEItemModel() { }

QGraphicsScene *VEItemModel::scene() const
{
    return m_graphicsScene;
}

QGraphicsItem *VEItemModel::getItem(const Id id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return m_items.value(id);
}

shared::VEModel *shared::ui::VEItemModel::objectsModel() const
{
    return m_model;
}

void VEItemModel::zoomChanged()
{
    for (QGraphicsItem *item : m_graphicsScene->selectedItems()) {
        if (auto iObj = qobject_cast<VEInteractiveObject *>(item->toGraphicsObject())) {
            iObj->updateGripPoints();
        }
    }
}

void VEItemModel::clearScene()
{
    if (m_graphicsScene) {
        m_graphicsScene->clear();
    }
    m_items.clear();
    updateSceneRect();
}

void VEItemModel::onSceneSelectionChanged()
{
    QList<shared::Id> ids;
    for (auto item : m_graphicsScene->selectedItems()) {
        if (auto iObj = qobject_cast<VEInteractiveObject *>(item->toGraphicsObject())) {
            ids.append(iObj->entity()->id());
        }
    }
    Q_EMIT itemsSelected(ids);
}

void VEItemModel::onObjectAdded(shared::Id objectId)
{
    if (!m_graphicsScene || objectId.isNull()) {
        return;
    }

    VEInteractiveObject *item = m_items.value(objectId);
    if (!item) {
        item = createItemForObject(objectId);
        if (!item) {
            return;
        }
        initItem(item);
    }
    updateItem(item);
}

void VEItemModel::onObjectsAdded(const QVector<shared::Id> &objectsIds)
{
    for (const Id objectId : objectsIds) {
        onObjectAdded(objectId);
    }

    updateSceneRect();
}

void VEItemModel::onObjectRemoved(shared::Id objectId)
{
    if (!m_graphicsScene) {
        return;
    }

    m_rmQueu.enqueue(objectId);

    while (!m_rmQueu.isEmpty()) {
        if (m_mutex->tryLock()) {
            removeItemForObject(m_rmQueu.dequeue());
            m_mutex->unlock();
        }
    }
}

void VEItemModel::onObjectAttributeChanged()
{
    if (auto senderObject = qobject_cast<VEObject *>(sender())) {
        if (auto item = m_items.value(senderObject->id())) {
            updateItem(item);
        }
    }
}

VEInteractiveObject *VEItemModel::createItemForObject(shared::Id objectId)
{
    Q_ASSERT(!objectId.isNull());
    VEObject *obj = m_model->getObject(objectId);

    Q_ASSERT(obj);
    if (!obj) {
        return nullptr;
    }

    if (VEInteractiveObject *iObj = createItem(objectId)) {
        if (!iObj) {
            return nullptr;
        }
        if (const QGraphicsItem *parentItem = iObj->parentItem()) {
            if (auto iObjParent = qobject_cast<const VEInteractiveObject *>(parentItem->toGraphicsObject())) {
                connect(iObj, &VEInteractiveObject::boundingBoxChanged, iObjParent,
                        &VEInteractiveObject::scheduleLayoutUpdate, Qt::QueuedConnection);
            }
        }
        iObj->setCommandsStack(m_commandsStack);
        iObj->init();
        return iObj;
    }

    return nullptr;
}

void VEItemModel::initItem(VEInteractiveObject *item)
{
    Q_ASSERT(item);
    Q_ASSERT(item->entity());
    if (!item || !item->entity()) {
        return;
    }

    const VEObject *object = m_model->getObject(item->entity()->id());
    if (!object) {
        return;
    }

    connect(object, &VEObject::attributeChanged, this, &VEItemModel::onObjectAttributeChanged);
    connect(
            item, &VEInteractiveObject::clicked, this,
            [this, entity = item->entity()]() { Q_EMIT itemClicked(entity->id()); }, Qt::QueuedConnection);
    connect(
            item, &VEInteractiveObject::doubleClicked, this,
            [this, entity = item->entity()]() { Q_EMIT itemDoubleClicked(entity->id()); }, Qt::QueuedConnection);

    m_items.insert(object->id(), item);
    if (m_graphicsScene != item->scene()) {
        m_graphicsScene->addItem(item);
    }
}

void VEItemModel::updateItem(VEInteractiveObject *item)
{
    Q_ASSERT(item);
    if (!item) {
        return;
    }

    if (auto iObj = qobject_cast<VEInteractiveObject *>(item->toGraphicsObject())) {
        iObj->updateFromEntity();
    }
    if (m_mutex->tryLock()) {
        updateSceneRect();
        m_mutex->unlock();
    }
}

void VEItemModel::removeItemForObject(shared::Id objectId)
{
    if (auto item = m_items.take(objectId)) {
        m_graphicsScene->removeItem(item);
        delete item;
        updateSceneRect();
    }
}

void VEItemModel::updateSceneRect()
{
    if (!m_graphicsScene) {
        return;
    }

    const QRectF itemsRect = m_graphicsScene->itemsBoundingRect();
    if (itemsRect.isEmpty()) {
        m_prevItemsRect = {};
        m_graphicsScene->setSceneRect(m_desktopGeometry);
        return;
    }

    if (itemsRect != m_prevItemsRect) {
        const QRectF sceneRect = m_graphicsScene->sceneRect().marginsRemoved(shared::graphicsviewutils::kRootMargins);
        const QRectF updated = sceneRect.united(itemsRect);

        if (sceneRect != updated) {
            m_graphicsScene->setSceneRect(updated.marginsAdded(shared::graphicsviewutils::kRootMargins));
            m_prevItemsRect = itemsRect;
        }
    }
}

} // namespace ui
} // namespace shared
