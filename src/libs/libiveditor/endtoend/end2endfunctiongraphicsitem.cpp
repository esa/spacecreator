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

#include "end2endfunctiongraphicsitem.h"

#include "graphicsviewutils.h"

#include <QPainter>
#include <QtDebug>

namespace ive {

End2EndMiniViewRenderer::End2EndMiniViewRenderer(const End2EndFunctionGraphicsItem *item)
    : MiniViewRenderer(item)
{
}

void End2EndMiniViewRenderer::setEnd2EndConnections(const QList<shared::Id> &connections)
{
    m_connections = connections;
}

void End2EndMiniViewRenderer::render(QPainter *painter)
{
    MiniViewRenderer::render(painter);
    if (!d->item->entity()->hasNestedChildren()) {
        return;
    }

    painter->save();
    painter->setClipPath(d->item->shape());
    const shared::ColorHandler ch =
            shared::ColorManager::instance()->colorsForItem(shared::ColorManager::ConnectionFlow);
    painter->setBrush(ch.brush());
    painter->setPen(QPen(ch.penColor(), ch.penWidth()));

    QVector<QPointF> polyline;
    for (const shared::Id &connectionId : qAsConst(m_connections)) {
        const QPolygonF points = d->polygons.value(connectionId);
        polyline << points;
    }
    painter->drawPolyline(polyline);
    painter->restore();
}

End2EndFunctionGraphicsItem::End2EndFunctionGraphicsItem(ivm::IVFunction *entity, QGraphicsItem *parent)
    : IVFunctionGraphicsItem(entity, std::make_unique<End2EndMiniViewRenderer>(this), parent)
{
}

void End2EndFunctionGraphicsItem::setEnd2EndConnections(const QList<shared::Id> &connections)
{
    auto renderer = static_cast<End2EndMiniViewRenderer *>(m_miniViewDrawer.get());
    renderer->setEnd2EndConnections(connections);
}

} // namespace ive
