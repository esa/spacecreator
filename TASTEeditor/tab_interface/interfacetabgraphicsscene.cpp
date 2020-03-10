/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "interfacetabgraphicsscene.h"

#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"

#include <QGraphicsSceneHelpEvent>
#include <QToolTip>

namespace taste3 {

InterfaceTabGraphicsScene::InterfaceTabGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void InterfaceTabGraphicsScene::helpEvent(QGraphicsSceneHelpEvent *event)
{
    if (QGraphicsItem *hovered = utils::nearestItem(this, event->scenePos(), utils::knownGraphicsItemTypes())) {
        if (aadl::InteractiveObject *item = qobject_cast<aadl::InteractiveObject *>(hovered->toGraphicsObject())) {
            const QString &tooltip = item->prepareTooltip();
            if (!tooltip.isEmpty()) {
                QToolTip::showText(event->screenPos(), tooltip, event->widget());
                event->accept();
                return;
            }
        }
    }

    QGraphicsScene::helpEvent(event);
}

} // ns taste3
