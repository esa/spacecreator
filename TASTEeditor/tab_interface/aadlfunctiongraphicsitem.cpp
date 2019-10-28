/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlfunctiongraphicsitem.h"

#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "tab_aadl/aadlobjectfunction.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

static const qreal kBorderWidth = 2;

namespace taste3 {
namespace aadl {

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(AADLObjectFunction *entity, QGraphicsItem *parent)
    : AADLFunctionTypeGraphicsItem(entity, parent)
{
    setObjectName(QLatin1String("AADLFunctionGraphicsItem"));

    updateColors();
}

AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunction *>(dataObject());
}

void AADLFunctionGraphicsItem::rebuildLayout()
{
    AADLFunctionTypeGraphicsItem::rebuildLayout();
    updateConnections();
}

void AADLFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? m_selectedPen : m_pen);
    painter->setBrush(brush());
    painter->drawRoundedRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2), 10, 10);
    painter->restore();
}

QVariant AADLFunctionGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemParentHasChanged)
        updateColors();
    return AADLFunctionTypeGraphicsItem::itemChange(change, value);
}

void AADLFunctionGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualMoveProgress(grip, from, to);
    updateConnections();
}

void AADLFunctionGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualResizeProgress(grip, from, to);
    updateConnections();
}

QSizeF AADLFunctionGraphicsItem::minimalSize() const
{
    QRectF br;
    for (QGraphicsItem *item : childItems()) {
        if (item->type() == AADLFunctionGraphicsItem::Type)
            br |= item->boundingRect();
    }
    const QSizeF baseSize { AADLFunctionTypeGraphicsItem::minimalSize() };
    return { qMax(baseSize.width(), br.width()), qMax(baseSize.height(), br.height()) };
}

void AADLFunctionGraphicsItem::updateColors()
{
    QColor brushColor { QLatin1String("#ffd11a") };
    if (auto parentFunction = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem()))
        brushColor = parentFunction->brush().color().darker(125);
    setBrush(brushColor);
    setPen(QPen(brushColor.darker(), 2));
}

void AADLFunctionGraphicsItem::updateConnections()
{
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            if (AADLConnectionGraphicsItem *connection = iface->connectedItem())
                connection->instantLayoutUpdate();
        }
    }
}

void AADLFunctionGraphicsItem::updateTextPosition()
{
    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    QRectF textRect { 0, 0, m_textItem->boundingRect().width() / currScale.x(),
                      m_textItem->boundingRect().height() / currScale.y() };
    textRect.moveCenter(boundingRect().center());
    m_textItem->setPos(textRect.topLeft());
}

} // namespace aadl
} // namespace taste3
