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
#include <QtGlobal>

static const ivm::meta::Props::Token innerCoordToken = ivm::meta::Props::Token::InnerCoordinates;
static const QList<ivm::IVObject::Type> rectangleEntityTypes { ivm::IVObject::Type::Function,
    ivm::IVObject::Type::FunctionType, ivm::IVObject::Type::Comment };

namespace ive {

struct ConnectionData {
    QPointF outerMappedScenePos;
    QPointF innerScenePos;
    shared::Id innerFunctionId;
};

struct MiniViewRendererPrivate {
    explicit MiniViewRendererPrivate(const IVFunctionGraphicsItem *item)
        : item(item)
    {
    }

    const IVFunctionGraphicsItem *item { nullptr };

    QRectF nestedRect;
    QList<shared::Id> itemsWithoutGeometry;
    QHash<shared::Id, QRectF> existingRects;
    QHash<shared::Id, QPolygonF> childrenConnections;

    QList<ConnectionData> parentChildConnections;
};

MiniViewRenderer::MiniViewRenderer(const IVFunctionGraphicsItem *item)
    : d(std::make_unique<MiniViewRendererPrivate>(item))
{
}

MiniViewRenderer::~MiniViewRenderer() { }

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

    if (!d->nestedRect.isValid()) {
        drawItemsWithoutCoordinates(painter);
    } else {
        drawData(painter);
    }
    painter->restore();
}

void MiniViewRenderer::clear()
{
    d->nestedRect = {};
    d->itemsWithoutGeometry.clear();
    d->existingRects.clear();
    d->childrenConnections.clear();
    d->parentChildConnections.clear();
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

    for (const ivm::IVObject *child : qAsConst(childEntities)) {
        const QString strCoordinates = child->entityAttributeValue<QString>(ivm::meta::Props::token(innerCoordToken));
        if (rectangleEntityTypes.contains(child->type())) {
            const QRectF itemSceneRect =
                    shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(strCoordinates));
            if (itemSceneRect.isValid()) {
                d->nestedRect |= itemSceneRect;
                d->existingRects.insert(child->id(), itemSceneRect);
            } else {
                d->itemsWithoutGeometry.append(child->id());
            }
        } else if (auto connection = qobject_cast<const ivm::IVConnection *>(child)) {
            if (connection->source()->id() != d->item->entity()->id()
                    && connection->target()->id() != d->item->entity()->id()) { // Child <> Child Connection
                const QPolygonF itemScenePoints =
                        shared::graphicsviewutils::polygon(ivm::IVObject::coordinatesFromString(strCoordinates));
                if (!itemScenePoints.isEmpty()) {
                    d->childrenConnections.insert(child->id(), itemScenePoints);
                } else {
                    /// TODO:
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

                QPointF innerIfacePos;
                if (innerIface->hasEntityAttribute(ivm::meta::Props::token(innerCoordToken))) {
                    const QString ifaceStrCoordinates =
                            innerIface->entityAttributeValue<QString>(ivm::meta::Props::token(innerCoordToken));
                    innerIfacePos = ifaceStrCoordinates.isEmpty()
                            ? QPointF(-1, -1)
                            : shared::graphicsviewutils::pos(ivm::IVObject::coordinatesFromString(ifaceStrCoordinates));
                    const ConnectionData cd { outerIfaceItem->scenePos(), innerIfacePos,
                        innerIface->parentObject()->id() };
                    d->parentChildConnections << cd;
                }
            }
        }
    }
    while (!d->itemsWithoutGeometry.isEmpty()) {
        const shared::Id id = d->itemsWithoutGeometry.takeLast();
        QRectF itemRect { QPointF(), shared::graphicsviewutils::kDefaultGraphicsItemSize };
        shared::graphicsviewutils::findGeometryForRect(itemRect, d->nestedRect, d->existingRects.values());
        d->existingRects.insert(id, itemRect);
    }
}

static inline void drawRectangularItems(
        const QRectF &boundingRect, const QList<QRectF> &existingRects, int count, const qreal sf, QPainter *painter)
{
    QRectF childRect { QPointF(), shared::graphicsviewutils::kDefaultGraphicsItemSize * sf };
    const qreal yOffset = sf
            * (shared::graphicsviewutils::kContentMargins.top() + shared::graphicsviewutils::kContentMargins.bottom());
    const qreal xOffset = sf
            * (shared::graphicsviewutils::kContentMargins.left() + shared::graphicsviewutils::kContentMargins.right());
    const int column = boundingRect.width() / (childRect.width() + xOffset);
    const int row = boundingRect.height() / (childRect.height() + yOffset);
    if (!count || !column || !row) {
        return;
    }
    for (int idx = 0; idx < count && idx < column * row;) {
        const int currentRow = idx / column;
        const int currentColumn = idx % column;
        childRect.moveTopLeft(boundingRect.topLeft()
                + QPointF((childRect.width() + xOffset) * currentColumn, (childRect.height() + yOffset) * currentRow));
        auto it = std::find_if(existingRects.cbegin(), existingRects.cend(),
                [childRect](const QRectF &r) { return r.intersects(childRect); });
        if (it == existingRects.cend()) {
            painter->drawRect(childRect);
            ++idx;
        }
    }
};

void MiniViewRenderer::drawItemsWithoutCoordinates(QPainter *painter)
{
    auto view = d->item->scene()->views().value(0);
    if (!view)
        return;

    const QRectF br = d->item->boundingRect();

    const QRect viewportGeometry =
            view->viewport()->geometry().marginsRemoved(shared::graphicsviewutils::kContentMargins.toMargins());
    const QRectF mappedViewportGeometry =
            QRectF(view->mapToScene(viewportGeometry.topLeft()), view->mapToScene(viewportGeometry.bottomRight()));

    const qreal sf = qMin(br.width() / mappedViewportGeometry.width(), br.height() / mappedViewportGeometry.height());
    drawRectangularItems(br, {}, d->itemsWithoutGeometry.size(), sf, painter);
}

void MiniViewRenderer::drawData(QPainter *painter)
{
    const QRectF br = d->item->boundingRect();
    const QRectF contentRect { d->nestedRect.marginsAdded(shared::graphicsviewutils::kRootMargins) };
    const qreal sf = qMin(br.width() / contentRect.width(), br.height() / contentRect.height());
    const QTransform transform = QTransform::fromScale(sf, sf).translate(-contentRect.x(), -contentRect.y());

    QList<QRectF> mappedRects;
    QFont painterFont = painter->font();
    painterFont.setItalic(true);
    painterFont.setPointSize(painterFont.pointSize() - 1);
    const QFontMetricsF fm(painterFont);
    for (auto it = d->existingRects.cbegin(); it != d->existingRects.cend(); ++it) {
        const QRectF r = it.value();
        const QRectF mappedRect = transform.mapRect(r);
        mappedRects << mappedRect;
        painter->drawRect(mappedRect);

        const QString text = d->item->entity()->model()->getObject(it.key())->titleUI();
        const qreal margin = 8;
        painter->setFont(painterFont);
        shared::graphicsviewutils::drawText(painter, mappedRect.adjusted(margin, margin, -margin, -margin), text, margin);
    }
    for (const QPolygonF &p : qAsConst(d->childrenConnections)) {
        painter->drawPolyline(transform.map(p));
    }
    for (const ConnectionData &connectionData : qAsConst(d->parentChildConnections)) {
        const QRectF innerRect = transform.mapRect(d->existingRects.value(connectionData.innerFunctionId));
        const QRectF outerRect = d->item->mapRectFromScene(d->item->sceneBoundingRect());
        const QPointF outerPos = d->item->mapFromScene(connectionData.outerMappedScenePos);
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
        painter->drawPolyline(
                shared::graphicsviewutils::createConnectionPath(mappedRects, outerPos, outerRect, innerPos, innerRect));
    }
}

} // namespace ive
