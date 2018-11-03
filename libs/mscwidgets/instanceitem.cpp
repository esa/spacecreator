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

#include "instanceitem.h"
#include "messageitem.h"
#include "baseitems/textitem.h"
#include "baseitems/arrowitem.h"
#include "baseitems/grippoint.h"
#include "baseitems/grippointshandler.h"

#include <mscinstance.h>

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QApplication>
#include <QTextDocument>

#include <QDebug>
#define LOG qDebug() << Q_FUNC_INFO

namespace msc {

static const double SYMBOLS_WIDTH = 60.0;
static const double START_SYMBOL_HEIGHT = 20.0;
static const double END_SYMBOL_HEIGHT = 15.0;

InstanceItem::InstanceItem(msc::MscInstance *instance, QGraphicsItem *parent)
    : InteractiveObject(parent)
    , m_instance(instance)
    , m_headSymbol(new QGraphicsRectItem(this))
    , m_nameItem(new TextItem(this))
    , m_kindItem(new TextItem(this))
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_endSymbol(new QGraphicsRectItem(this))
{
    Q_ASSERT(m_instance != nullptr);
    m_endSymbol->setBrush(QBrush(Qt::black));

    setName(m_instance->name());
    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);

    setKind(m_instance->kind());
    connect(m_instance, &msc::MscInstance::kindChanged, this, &msc::InstanceItem::setKind);

    connect(this, &msc::InteractiveObject::resized, this, &msc::InstanceItem::onResized);
    updateLayout();

    m_headSymbol->setZValue(m_gripPoints->zValue() - 1);
    m_nameItem->setZValue(m_gripPoints->zValue() - 1);
    m_kindItem->setZValue(m_gripPoints->zValue() - 1);
    m_axisSymbol->setZValue(m_gripPoints->zValue() - 1);
    m_endSymbol->setZValue(m_gripPoints->zValue() - 1);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges);
}

void InstanceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

MscInstance *InstanceItem::modelItem() const
{
    return m_instance;
}

QString InstanceItem::name() const
{
    return m_nameItem->toPlainText();
}

QString InstanceItem::kind() const
{
    return m_kindItem->toPlainText();
}

void InstanceItem::setAxisHeight(double height)
{
    if (qFuzzyCompare(m_axisHeight, height)) {
        return;
    }
    m_axisHeight = height;
    updateLayout();
}

void InstanceItem::updateLayout()
{
    if (m_layoutDirty) {
        return;
    }

    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "buildLayout", Qt::QueuedConnection);
}

void InstanceItem::setName(const QString &name)
{
    if (this->name() == name)
        return;

    m_nameItem->setPlainText(name);
    m_nameItem->adjustSize();
    m_instance->setName(name);
    updateLayout();
}

void InstanceItem::setKind(const QString &kind)
{
    if (this->kind() == kind)
        return;

    m_kindItem->setPlainText(kind);
    m_kindItem->adjustSize();
    m_instance->setKind(kind);
    updateLayout();
}

QVariant InstanceItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemPositionChange: {
        onRelocated(value.toPointF() - pos());
        break;
    }
    default:
        break;
    }

    return InteractiveObject::itemChange(change, value);
}

void InstanceItem::buildLayout()
{
    QRectF nameRect({ 0., 0. }, m_nameItem->boundingRect().size());
    const QRectF kindRect(m_kindItem->boundingRect());
    QRectF kindR(nameRect.bottomLeft(),
                 QSizeF(qMax(kindRect.width(), qMax(nameRect.width(), SYMBOLS_WIDTH)), qMax(kindRect.height(), START_SYMBOL_HEIGHT)));

    // precalculate own default size:
    if (m_boundingRect.isEmpty()) {
        m_boundingRect.setTopLeft(nameRect.topLeft());
        m_boundingRect.setWidth(qMax(nameRect.width(), kindR.width()));
        m_boundingRect.setHeight(nameRect.height() + kindR.height() + m_axisHeight + END_SYMBOL_HEIGHT);
    }

    // move name to the top:
    nameRect.moveTopLeft(m_boundingRect.topLeft());
    const QPointF nameDelta = nameRect.center() - m_nameItem->boundingRect().center();
    m_nameItem->setPos({ 0., 0. });
    m_nameItem->moveBy(nameDelta.x(), nameDelta.y());

    // move head symb and kind txt below name:
    kindR.moveTop(m_nameItem->boundingRect().bottom());
    kindR.setWidth(m_boundingRect.width());
    m_kindItem->setPos(m_nameItem->boundingRect().translated(m_nameItem->pos()).bottomLeft());
    QRectF headRect = m_kindItem->boundingRect().translated(m_kindItem->pos());
    headRect.setWidth(m_boundingRect.width());
    m_headSymbol->setRect(headRect);

    // move end symb to the bottom:
    QRectF footerRect(m_boundingRect.left(), m_boundingRect.bottom() - END_SYMBOL_HEIGHT, m_boundingRect.width(), END_SYMBOL_HEIGHT);
    m_endSymbol->setRect(footerRect);

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));

    for (QGraphicsItem *ci : childItems())
        if (QGraphicsTextItem *ti = dynamic_cast<QGraphicsTextItem *>(ci))
            ti->setTextWidth(m_boundingRect.width());

    m_layoutDirty = false;
}

void InstanceItem::rememberMessageItem(MessageItem *msg, QVector<MessageItem *> &container)
{
    if (!msg || container.contains(msg))
        return;

    container.append(msg);
}

void InstanceItem::forgetMessageItem(MessageItem *msg, QVector<MessageItem *> &container)
{
    if (!msg)
        return;
    container.removeAll(msg);
}

void InstanceItem::registerIncoming(MessageItem *rxMsg)
{
    rememberMessageItem(rxMsg, m_rxMessages);
}

void InstanceItem::unregisterIncoming(MessageItem *rxMsg)
{
    forgetMessageItem(rxMsg, m_rxMessages);
}

void InstanceItem::registerOutgoing(MessageItem *txMsg)
{
    rememberMessageItem(txMsg, m_txMessages);
}

void InstanceItem::unregisterOutgoing(MessageItem *txMsg)
{
    forgetMessageItem(txMsg, m_txMessages);
}

void InstanceItem::onRelocated(const QPointF &delta)
{
    for (MessageItem *msg : m_txMessages)
        msg->updateAnchorSource(delta);

    for (MessageItem *msg : m_rxMessages)
        msg->updateAnchorTarget(delta);
}

void InstanceItem::onResized(const QRectF &from, const QRectF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    // TODO: move arrows of appropriate incoming/outgoing messages, if necessary

    updateLayout();
}

QPainterPath InstanceItem::shape() const
{
    QPainterPath res;
    res.addRect(m_headSymbol->boundingRect());
    res.addRect(m_nameItem->boundingRect());
    res.addRect(m_kindItem->boundingRect());
    res.addRect(m_endSymbol->boundingRect());
    res.addPath(ArrowItem::lineShape(m_axisSymbol->line(), InteractiveObject::SPAN));
    return res;
}

} // namespace msc
