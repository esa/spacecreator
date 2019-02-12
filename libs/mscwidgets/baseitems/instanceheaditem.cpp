/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "instanceheaditem.h"

#include "baseitems/textitem.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace msc {

static constexpr qreal SymbolWidth = { 60.0 };
const qreal InstanceHeadItem::StartSymbolHeight = 20.;

QLinearGradient InstanceHeadItem::createGradientForKind(const QGraphicsItem *itemKind)
{
    static QLinearGradient prototype;
    if (!itemKind)
        return prototype;

    static bool prototypeFilled(false);

    if (!prototypeFilled) {
        // colors were colorpicked from https://git.vikingsoftware.com/esa/msceditor/issues/30
        prototype.setColorAt(0.0, QColor("#fefef9"));
        prototype.setColorAt(0.5, QColor("#fefeca"));
        prototype.setColorAt(1.0, QColor("#dedbb4"));
        prototypeFilled = true;
    }

    QLinearGradient gradient(prototype);
    const QRectF &bounds = itemKind->boundingRect();
    gradient.setStart(bounds.topLeft());
    gradient.setFinalStop(bounds.bottomRight());
    return gradient;
}

QLinearGradient InstanceHeadItem::createGradientForName(const QGraphicsItem *itemName)
{
    static QLinearGradient prototype;
    if (!itemName)
        return prototype;

    static bool prototypeFilled(false);

    if (!prototypeFilled) {
        const QColor &whiteTransparent(QColor::fromRgbF(1., 1., 1., 0.25));
        prototype.setColorAt(0.0, whiteTransparent);
        prototype.setColorAt(0.5, Qt::white);
        prototype.setColorAt(1.0, whiteTransparent);
        prototypeFilled = true;
    }

    QLinearGradient gradient(prototype);
    const QRectF &bounds = itemName->boundingRect();
    gradient.setStart(bounds.topLeft());
    gradient.setFinalStop(bounds.topRight());
    return gradient;
}

InstanceHeadItem::InstanceHeadItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_textItemName(new TextItem(this))
    , m_rectItem(new QGraphicsRectItem(this))
    , m_textItemKind(new TextItem(this))
{
    m_textItemKind->setBackgroundColor(Qt::transparent);
    m_textItemKind->setEditable(true);
    m_textItemName->setEditable(true);

    connect(m_textItemName, &TextItem::edited, this, [this](const QString &txt) {
        updateLayout();
        Q_EMIT nameEdited(txt);
    });

    connect(m_textItemName, &TextItem::keyPressed, this, [this]() { updateLayout(); });

    connect(m_textItemKind, &TextItem::edited, this, [this](const QString &txt) {
        updateLayout();
        Q_EMIT kindEdited(txt);
    });
    connect(m_textItemKind, &TextItem::keyPressed, this, [this]() { updateLayout(); });

    m_rectItem->setCursor(Qt::SizeAllCursor);
    m_textItemKind->setCursor(Qt::SizeAllCursor); // TODO: restore regular cursor for editing
}

QString InstanceHeadItem::name() const
{
    return m_textItemName->toPlainText();
}

QString InstanceHeadItem::kind() const
{
    return m_textItemKind->toPlainText();
}

void InstanceHeadItem::setName(const QString &name)
{
    if (name == this->name())
        return;

    m_textItemName->setPlainText(name);
    m_textItemName->adjustSize();

    updateLayout();
}

void InstanceHeadItem::setKind(const QString &kind)
{
    if (kind == this->kind())
        return;

    m_textItemKind->setPlainText(kind);
    m_textItemKind->adjustSize();

    updateLayout();
}

void InstanceHeadItem::updateLayout()
{
    prepareGeometryChange();

    QRectF nameRect = m_textItemName->boundingRect();
    QRectF kindRect = m_textItemKind->boundingRect();

    // prepare symbol's rect:
    const qreal width = qMax(SymbolWidth, qMax(nameRect.width(), kindRect.width()));
    QRectF symbolRect(0., 0., width, qMax(kindRect.height(), StartSymbolHeight));

    static const qreal padding = 5.;
    symbolRect.adjust(-padding, -padding, padding, padding);

    symbolRect.moveTopLeft({ 0., 0. });

    // center name horizontaly:
    nameRect.moveCenter(symbolRect.center());

    // shift box downward:
    symbolRect.moveTop(nameRect.bottom());
    m_rectItem->setRect(symbolRect);

    // center kind in the box:
    kindRect.moveCenter(symbolRect.center());

    // place items:
    auto moveItem = [](QGraphicsItem *item, const QPointF &newCenter) {
        const QRectF &currBounds = item->boundingRect().translated(item->pos());
        const QPointF &currCenter = currBounds.center();
        const QPointF &shift = newCenter - currCenter;
        item->moveBy(shift.x(), shift.y());
    };

    moveItem(m_textItemName, nameRect.center());
    moveItem(m_textItemKind, kindRect.center());
    moveItem(m_rectItem, symbolRect.center());

    // update head gradient:
    m_textItemName->setBackgroundGradient(createGradientForName(m_textItemName));
    m_rectItem->setBrush(createGradientForKind(m_rectItem));

    Q_EMIT layoutUpdated();
}

QRectF InstanceHeadItem::boundingRect() const
{
    return childrenBoundingRect();
}

void InstanceHeadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void InstanceHeadItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_manualMovementFrom =
            (!m_textItemKind->isEditing() && m_rectItem->contains(event->pos())) ? event->pos() : QPointF();

    m_manualMovementTo = m_manualMovementFrom;

    event->accept();

    if (QGraphicsItem *parent = parentItem()) {

        if (event->modifiers() == Qt::NoModifier)
            scene()->clearSelection();

        parent->setSelected(!parent->isSelected());
    }
}

void InstanceHeadItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_manualMovementFrom != m_manualMovementTo) {
        emit manualMoveFinished();
    }

    m_manualMovementFrom = m_manualMovementTo = QPointF();
}

void InstanceHeadItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    auto activateTextEdit = [](TextItem *textItem, const QPointF &scenePos) {
        if (textItem->contains(textItem->mapFromScene(scenePos))) {
            textItem->enableEditMode();
            return true;
        }
        return false;
    };

    if (activateTextEdit(m_textItemKind, event->scenePos()) || activateTextEdit(m_textItemName, event->scenePos())) {
        m_manualMovementFrom = m_manualMovementTo = QPointF();
    }
}

void InstanceHeadItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_manualMovementFrom.isNull() && event->lastScenePos() != event->scenePos()) {
        m_manualMovementTo = event->scenePos();
        Q_EMIT manualMoveRequested(event->lastScenePos(), event->scenePos());
    }
}

} // ns msc
