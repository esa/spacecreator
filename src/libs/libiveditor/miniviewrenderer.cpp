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

#include "miniviewrenderer.h"

#include "graphicsviewutils.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "ivmodel.h"

#include <QGraphicsView>
#include <QPainter>
#include <QtDebug>
#include <QtGlobal>

static const QList<ivm::IVObject::Type> rectangleEntityTypes { ivm::IVObject::Type::Function,
    ivm::IVObject::Type::FunctionType, ivm::IVObject::Type::Comment };

namespace ive {

MiniViewRenderer::MiniViewRenderer(const IVFunctionGraphicsItem *item)
    : d(std::make_unique<MiniViewRendererPrivate>(item))
{
}

MiniViewRenderer::~MiniViewRenderer() {}

void MiniViewRenderer::render(QPainter *painter)
{
    if (!d->item->entity()->hasNestedChildren()) {
        return;
    }

    updateData();

    painter->save();
    painter->setClipPath(d->item->shape());
    const shared::ColorHandler ch =
            shared::ColorManager::instance()->colorsForItem(shared::ColorManager::FunctionScale);
    painter->setBrush(ch.brush());
    painter->setPen(QPen(ch.penColor(), ch.penWidth()));

    QFont painterFont = painter->font();
    painterFont.setItalic(true);
    painterFont.setPointSize(painterFont.pointSize() - 1);
    painter->setFont(painterFont);
    for (auto it = d->rects.cbegin(); it != d->rects.cend(); ++it) {
        painter->drawRect(it.value());

        const QString text = d->item->entity()->model()->getObject(it.key())->titleUI();
        static const qreal margin = 8;
        shared::graphicsviewutils::drawText(
                painter, it.value().adjusted(margin, margin, -margin, -margin), text, margin);
    }
    for (auto it = d->polygons.cbegin(); it != d->polygons.cend(); ++it) {
        painter->drawPolyline(it.value());
    }
    painter->restore();
}

void MiniViewRenderer::clear()
{
    d->rects.clear();
    d->polygons.clear();
}

template<typename T>
static const T *findChildItem(const QGraphicsItem *item, const shared::Id &id)
{
    const QList<QGraphicsItem *> items = item->childItems();
    auto it = std::find_if(items.cbegin(), items.cend(), [id](const QGraphicsItem *item) {
        if (item->type() != T::Type) {
            return false;
        } else if (auto iface = qgraphicsitem_cast<const T *>(item)) {
            return iface->entity()->id() == id;
        } else {
            return false;
        }
    });
    return it == items.cend() ? nullptr : qgraphicsitem_cast<const T *>(*it);
}

void MiniViewRenderer::updateData()
{
    clear();

    const QVector<ivm::IVObject *> childEntities = d->item->entity()->children();

    QRectF nestedRect;
    QList<shared::Id> itemsWithoutGeometry;
    QHash<shared::Id, QRectF> rectItems;
    QHash<shared::Id, QPolygonF> polygonItems;

    struct ConnectionData {
        QPointF outerMappedScenePos;
        QPointF innerScenePos;
        shared::Id innerFunctionId;
        shared::Id connectionId;
        bool outerSource;
    };
    QList<ConnectionData> parentChildConnections;

    for (const ivm::IVObject *child : qAsConst(childEntities)) {
        auto token = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);
        const QString strCoordinates = child->entityAttributeValue<QString>(token);
        if (rectangleEntityTypes.contains(child->type())) {
            const QRectF itemSceneRect =
                    shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(strCoordinates));
            if (itemSceneRect.isValid()) {
                nestedRect |= itemSceneRect;
                rectItems.insert(child->id(), itemSceneRect);
            } else {
                itemsWithoutGeometry.append(child->id());
            }
        } else if (auto connection = qobject_cast<const ivm::IVConnection *>(child)) {
            const QPolygonF itemScenePoints =
                    shared::graphicsviewutils::polygon(ivm::IVObject::coordinatesFromString(strCoordinates));

            if (connection->source()->id() != d->item->entity()->id()
                    && connection->target()->id() != d->item->entity()->id()) { // Child <> Child Connection
                if (!itemScenePoints.isEmpty()) {
                    polygonItems.insert(child->id(), itemScenePoints);
                } else {
                    /// TODO: generate paths for connections between children
                }
            } else { // Parent <> Child connection
                const ivm::IVInterface *outerIface = connection->source()->id() == d->item->entity()->id()
                        ? connection->sourceInterface()
                        : connection->target()->id() == d->item->entity()->id() ? connection->targetInterface()
                                                                                : nullptr;

                const ivm::IVInterface *innerIface = connection->source()->id() == d->item->entity()->id()
                        ? connection->targetInterface()
                        : connection->target()->id() == d->item->entity()->id() ? connection->sourceInterface()
                                                                                : nullptr;

                if (!outerIface || !innerIface) {
                    continue;
                }

                const IVInterfaceGraphicsItem *outerIfaceItem =
                        findChildItem<IVInterfaceGraphicsItem>(d->item, outerIface->id());
                if (!outerIfaceItem) {
                    continue;
                }
                QPointF innerIfacePos;
                const QString innerIfacePosStr = token;
                if (innerIface->hasEntityAttribute(innerIfacePosStr)) {
                    const QString ifaceStrCoordinates = innerIface->entityAttributeValue<QString>(innerIfacePosStr);
                    innerIfacePos = ifaceStrCoordinates.isEmpty()
                            ? QPointF(-1, -1)
                            : shared::graphicsviewutils::pos(ivm::IVObject::coordinatesFromString(ifaceStrCoordinates));
                    const ConnectionData cd { outerIfaceItem->scenePos(), innerIfacePos,
                        innerIface->parentObject()->id(), connection->id(),
                        outerIface->id() == connection->sourceInterface()->id() };
                    parentChildConnections << cd;
                } else {
                    /// TODO: generate position for inner interface and path for connection between it
                    /// and outer interface
                }
            }
        }
    }
    while (!itemsWithoutGeometry.isEmpty()) {
        const shared::Id id = itemsWithoutGeometry.takeLast();
        QRectF itemRect { QPointF(), shared::graphicsviewutils::kDefaultGraphicsItemSize };
        shared::graphicsviewutils::findGeometryForRect(itemRect, nestedRect, d->rects.values());
        rectItems.insert(id, itemRect);
    }

    const QRectF br = d->item->boundingRect();
    const QRectF contentRect { nestedRect.marginsAdded(shared::graphicsviewutils::kRootMargins) };
    const qreal sf = qMin(br.width() / contentRect.width(), br.height() / contentRect.height());
    const QTransform transform = QTransform::fromScale(sf, sf).translate(-contentRect.x(), -contentRect.y());

    QList<QRectF> mappedRects;
    for (auto it = rectItems.cbegin(); it != rectItems.cend(); ++it) {
        const QRectF r = it.value();
        const QRectF mappedRect = transform.mapRect(r);
        d->rects[it.key()] = mappedRect;
    }
    for (auto it = polygonItems.cbegin(); it != polygonItems.cend(); ++it) {
        d->polygons[it.key()] = transform.map(it.value());
    }
    for (const ConnectionData &connectionData : qAsConst(parentChildConnections)) {
        QRectF innerRect = transform.mapRect(d->rects.value(connectionData.innerFunctionId));
        QRectF outerRect = d->item->mapRectFromScene(d->item->sceneBoundingRect());
        QPointF outerPos = d->item->mapFromScene(connectionData.outerMappedScenePos);
        QPointF innerPos;
        if (connectionData.innerScenePos == QPointF(-1, -1)) {
            const QPointF ratio { (outerRect.right() - outerPos.x()) / outerRect.width(),
                (outerRect.bottom() - outerPos.y()) / outerRect.height() };
            const qreal x = innerRect.left() + innerRect.width() * ratio.x();
            const qreal y = innerRect.top() + innerRect.height() * ratio.y();
            const Qt::Alignment side = shared::graphicsviewutils::getNearestSide(outerRect, outerPos);
            innerPos = shared::graphicsviewutils::getSidePosition(innerRect, QPointF(x, y), side);
        } else {
            innerPos = transform.map(connectionData.innerScenePos);
        }
        if (!connectionData.outerSource) {
            std::swap(outerPos, innerPos);
            std::swap(outerRect, innerRect);
        }
        d->polygons[connectionData.connectionId] =
                shared::graphicsviewutils::createConnectionPath(mappedRects, outerPos, outerRect, innerPos, innerRect);
    }
}

} // namespace ive
