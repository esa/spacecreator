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

#include <QObject>
#include <QQueue>
#include <QRectF>
#include <aadlobjectsmodel.h>

class QMutex;
class QGraphicsItem;
class QGraphicsScene;
class QItemSelectionModel;
class QItemSelection;

namespace aadl {
class AADLObject;
} // namespace aadl

namespace aadlinterface {
class InterfaceTabGraphicsScene;
class AADLCommentGraphicsItem;
class AADLInterfaceGraphicsItem;
class AADLFunctionGraphicsItem;
class AADLFunctionTypeGraphicsItem;
class AADLConnectionGraphicsItem;

/*!
   \class AADLItemModel is the model represents the scene graph of the currently selected/visible
   AADL model. It handles all changes with existing entities and reflects them on graphic scene.
 */

class AADLItemModel : public aadl::AADLObjectsModel
{
    Q_OBJECT
public:
    enum class AADLRoles
    {
        IdRole = Qt::UserRole,
        ItemVisible,
    };
    Q_ENUM(AADLRoles);

    explicit AADLItemModel(QObject *parent = nullptr);
    ~AADLItemModel() override;

    QGraphicsScene *scene() const;
    QItemSelectionModel *selectionModel() const;

    void changeRootItem(shared::Id id);
    void zoomChanged();

    QGraphicsItem *getItem(const shared::Id id) const;

Q_SIGNALS:
    void itemClicked(shared::Id id);
    void itemDoubleClicked(shared::Id id);

public Q_SLOTS:
    void updateSceneRect();
    void clearScene();

private Q_SLOTS:
    void onAADLObjectAdded(aadl::AADLObject *object);
    void onAADLObjectRemoved(aadl::AADLObject *object);
    void onRootObjectChanged(shared::Id rootId);
    void onConnectionAddedToGroup(aadl::AADLObjectConnection *connection);
    void onConnectionRemovedFromGroup(aadl::AADLObjectConnection *connection);

    void onSceneSelectionChanged();
    void onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QGraphicsItem *createItemForObject(aadl::AADLObject *obj);
    AADLFunctionGraphicsItem *rootItem() const;
    void updateItem(QGraphicsItem *item);
    void removeItemForObject(aadl::AADLObject *object);

private:
    QItemSelectionModel *m_itemSelectionModel;
    InterfaceTabGraphicsScene *m_graphicsScene { nullptr };
    QHash<shared::Id, QGraphicsItem *> m_items;
    QMutex *m_mutex { nullptr };
    QQueue<aadl::AADLObject *> m_rmQueu;
    QRectF m_desktopGeometry;
    QRectF m_prevItemsRect;
};

} // namespace aadlinterface
