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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "commentitem.h"

#include "baseitems/common/objectslink.h"
#include "objectslinkitem.h"
#include "textitem.h"

#include <QPainter>

static const qreal kBorderWidth = 1;
static const qreal kMargins = 6 + kBorderWidth;
static const qreal kLineLength = 20;

namespace msc {

CommentItem::CommentItem(QGraphicsItem *parent)
    : InteractiveObject(nullptr, parent)
    , m_textItem(new TextItem(this))
    , m_linkItem(new QGraphicsLineItem(this))
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setTextAllignment(Qt::AlignCenter);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    connect(m_textItem, &TextItem::edited, this, &CommentItem::textEdited);

    m_linkItem->setPen(QPen(Qt::black, kBorderWidth, Qt::DashLine));

    setHighlightable(false);
}

void CommentItem::setText(const QString &text)
{
    if (m_textItem->toPlainText() == text)
        return;

    m_textItem->setTextWidth(150);
    m_textItem->setPlainText(text);
    m_textItem->setTextWidth(m_textItem->idealWidth());

    textEdited(text);
}

QString CommentItem::text() const
{
    return m_textItem->toPlainText();
}

void CommentItem::attachTo(InteractiveObject *iObj)
{
    if (m_iObj == iObj)
        return;

    if (m_iObj)
        m_iObj->disconnect(this);

    m_iObj = iObj;

    connect(m_iObj, &InteractiveObject::moved, this, &CommentItem::updateLayout, Qt::UniqueConnection);
    connect(m_iObj, &InteractiveObject::relocated, this, &CommentItem::updateLayout, Qt::UniqueConnection);

    m_isGlobal = bool(m_iObj == nullptr);
    updateLayout();
}

InteractiveObject *CommentItem::object() const
{
    return m_iObj;
}

void CommentItem::setGlobal(bool isGlobal)
{
    m_isGlobal = isGlobal;
    updateLayout();
}

void CommentItem::updateLayout()
{
    rebuildLayout();
}

void CommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();
    pen.setWidthF(kBorderWidth);
    painter->setPen(pen);
    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    if (m_isGlobal) {
        painter->setBrush(QColor(0xf9e29c));
        painter->drawPolygon(QVector<QPointF> { br.topRight() - QPointF(kMargins, 0), br.topLeft(), br.bottomLeft(),
                                                br.bottomRight(), br.topRight() + QPointF(0, kMargins),
                                                br.topRight() - QPointF(kMargins, -kMargins),
                                                br.topRight() - QPointF(kMargins, 0) });
        painter->drawLine(br.topRight() - QPointF(kMargins, 0), br.topRight() + QPointF(0, kMargins));
    } else {
        painter->fillRect(br, QColor(0xf9e29c));
        if (m_inverseLayout)
            painter->drawPolyline(QVector<QPointF> { br.topLeft(), br.topRight(), br.bottomRight(), br.bottomLeft() });
        else
            painter->drawPolyline(QVector<QPointF> { br.topRight(), br.topLeft(), br.bottomLeft(), br.bottomRight() });
    }
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void CommentItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({});
}

void CommentItem::rebuildLayout()
{
    prepareGeometryChange();
    if (m_isGlobal) {
        m_linkItem->setVisible(false);
        m_boundingRect = m_textItem->boundingRect();
        setPos(sceneBoundingRect().topLeft());
        return;
    }

    const QPair<QPointF, bool> commentData = m_iObj ? m_iObj->commentPoint() : qMakePair(QPointF(0, 0), false);
    const QPointF commentPosition = commentData.first;
    m_inverseLayout = commentData.second;
    QRectF br = m_textItem->boundingRect();
    QLineF line;
    if (m_inverseLayout) { // inverse layouting for comment
        line = { br.right(), br.center().y(), br.right() + kLineLength, br.center().y() };
    } else {
        line = { br.left(), br.center().y(), br.left() + kLineLength, br.center().y() };
        br.moveLeft(line.p2().x());
    }
    m_linkItem->setVisible(true);
    m_linkItem->setLine(line);
    m_textItem->setPos(br.topLeft());

    m_boundingRect = mapRectFromItem(m_textItem, m_textItem->boundingRect())
            | mapRectFromItem(m_linkItem, m_linkItem->boundingRect());

    QRectF rect = sceneBoundingRect();
    rect.moveCenter(commentPosition);
    if (m_inverseLayout)
        rect.moveRight(commentPosition.x());
    else
        rect.moveLeft(commentPosition.x());
    setPos(rect.topLeft());
}

void CommentItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void CommentItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void CommentItem::textEdited(const QString &text)
{
    updateLayout();

    Q_EMIT commentChanged(text);
}

void CommentItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_textItem->enableEditMode();
    InteractiveObject::mouseDoubleClickEvent(event);
}

QPainterPath CommentItem::shape() const
{
    return m_textItem->shape() + m_linkItem->shape();
}

} // ns msc
