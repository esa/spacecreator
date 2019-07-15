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

#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectfunction.h"

#include <QApplication>
#include <QPainter>

static const qreal kBorderWidth = 2;

namespace taste3 {
namespace aadl {

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(AADLObjectFunction *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
{
    setObjectName(QLatin1String("AADLFunctionGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    QColor brushColor { QLatin1String("#ffd11a") };
    if (auto parentContainer = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem()))
        brushColor = parentContainer->brush().color().darker();
    setBrush(brushColor);
    setPen(QPen(brushColor.darker(), 2));
    setFont(QFont(qApp->font().family(), 16, QFont::Bold, true));
}

AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunction *>(m_entity);
}

void AADLFunctionGraphicsItem::rebuildLayout()
{
    if (auto graphicsItemParent = parentItem()) {
        QPointF newPos = pos();
        if (parentItem()) {
            const QRectF contentRect = graphicsItemParent->boundingRect();
            if (newPos.x() < contentRect.left())
                newPos.setX(contentRect.left());
            else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
                newPos.setX(contentRect.right() - m_boundingRect.width());
            if (newPos.y() < contentRect.top())
                newPos.setY(contentRect.top());
            else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
                newPos.setY(contentRect.bottom() - m_boundingRect.height());
            setPos(newPos);
        } else {
            setPos(newPos);
        }
    }
    InteractiveObject::rebuildLayout();
}

void AADLFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRoundedRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2), 10, 10);
    painter->setFont(font());
    painter->drawText(boundingRect(), Qt::AlignCenter, entity()->title());
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

} // namespace aadl
} // namespace taste3
