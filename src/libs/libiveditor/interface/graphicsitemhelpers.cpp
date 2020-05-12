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

#include "graphicsitemhelpers.h"

#include "aadlobject.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "baseitems/common/utils.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"

#include <QGraphicsScene>

namespace aadlinterface {
namespace gi {

aadl::AADLObjectFunction *functionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

aadl::AADLObjectFunctionType *functionTypeObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionTypeGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

aadl::AADLObjectIface *interfaceObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLInterfaceGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

aadl::AADLObjectComment *commentObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLCommentGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

aadl::AADLObjectConnection *connectionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLConnectionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

bool isOwnConnection(const QGraphicsItem *owner, const QGraphicsItem *connection)
{
    if (!owner || !connection)
        return false;

    if (auto con = qobject_cast<const AADLConnectionGraphicsItem *>(connection->toGraphicsObject()))
        for (auto item : owner->childItems())
            if (AADLInterfaceGraphicsItem::Type == item->type())
                if (con->startItem() == item || con->endItem() == item)
                    return true;

    return false;
}

bool canPlaceRect(QGraphicsScene *scene, const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect,
        const RectOperation action)
{
    if (!scene || upcomingItemRect.isEmpty() || !upcomingItem)
        return false;

    for (auto item : scene->items(upcomingItemRect)) {
        if (item == upcomingItem || item->type() < QGraphicsItem::UserType)
            continue;

        const int itemType = item->type();
        if (itemType <= QGraphicsItem::UserType)
            continue;
        if (AADLConnectionGraphicsItem::Type == itemType && isOwnConnection(upcomingItem, item))
            continue;

        if (action == RectOperation::Edit && upcomingItem->parentItem() == item->parentItem())
            return false;
    }

    return true;
}

}
}
