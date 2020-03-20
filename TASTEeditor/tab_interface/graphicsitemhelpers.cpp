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

#include "baseitems/common/utils.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectcomment.h"
#include "tab_aadl/aadlobjectconnection.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_interface/aadlcommentgraphicsitem.h"
#include "tab_interface/aadlconnectiongraphicsitem.h"
#include "tab_interface/aadlfunctiongraphicsitem.h"
#include "tab_interface/aadlfunctiontypegraphicsitem.h"
#include "tab_interface/aadlinterfacegraphicsitem.h"

#include <QGraphicsScene>

namespace taste3 {
namespace aadl {
namespace gi {

AADLObjectFunction *functionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

AADLObjectFunctionType *functionTypeObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionTypeGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

AADLObjectIface *interfaceObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLInterfaceGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

AADLObjectComment *commentObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLCommentGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

AADLObjectConnection *connectionObject(QGraphicsItem *item)
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
        if (item == upcomingItem)
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
} // ns gi
} // ns aadl
} // ns taste3
