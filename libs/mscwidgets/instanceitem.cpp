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

#include <mscinstance.h>

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

namespace msc {

static const double SYMBOLS_WIDTH = 60.0;
static const double START_SYMBOL_HEIGHT = 20.0;
static const double END_SYMBOL_HEIGHT = 15.0;

InstanceItem::InstanceItem(msc::MscInstance *instance, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_instance(instance)
    , m_headSymbol(new QGraphicsRectItem(this))
    , m_nameItem(new QGraphicsTextItem(this))
    , m_kindItem(new QGraphicsTextItem(this))
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_endSymbol(new QGraphicsRectItem(this))
{
    Q_ASSERT(m_instance != nullptr);
    m_endSymbol->setBrush(QBrush(Qt::black));

    setName(m_instance->name());
    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);

    setKind(m_instance->kind());
    connect(m_instance, &msc::MscInstance::kindChanged, this, &msc::InstanceItem::setKind);

    updateLayout();
}

MscInstance *InstanceItem::modelItem() const
{
    return m_instance;
}

QRectF InstanceItem::boundingRect() const
{
    QRectF rect = childrenBoundingRect();
    rect.moveTo(pos());
    return rect;
}

void InstanceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

double InstanceItem::horizontalCenter() const
{
    return x() + m_axisSymbol->line().x1();
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
    if (m_axisHeight == height) {
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
    m_nameItem->setPlainText(name);
    updateLayout();
    m_instance->setName(name);
}

void InstanceItem::setKind(const QString &kind)
{
    m_kindItem->setPlainText(kind);
    updateLayout();
    m_instance->setKind(kind);
}

QVariant InstanceItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        emit horizontalCenterChanged();
    }

    return QGraphicsObject::itemChange(change, value);
}

void InstanceItem::buildLayout()
{
    const double oldHorizontalZenter = horizontalCenter();
    QRectF nameRect = m_nameItem->boundingRect();

    const double centerX = std::max(nameRect.width(), SYMBOLS_WIDTH) / 2.0;

    const double nameX = centerX - nameRect.center().x();
    m_nameItem->setX(nameX);

    const double kindY = nameRect.bottom();
    QRectF kindRect = m_kindItem->boundingRect();
    const double kindX = centerX - kindRect.center().x();
    m_kindItem->setX(kindX);
    m_kindItem->setY(kindY);

    QRectF headSymbolRect = nameRect;
    headSymbolRect.moveTop(nameRect.bottom());
    const double headSymbolHeight = std::max(START_SYMBOL_HEIGHT, kindRect.height());
    headSymbolRect.setHeight(headSymbolHeight);
    headSymbolRect.setWidth(SYMBOLS_WIDTH);
    const double symbolX = centerX - headSymbolRect.center().x();
    headSymbolRect.moveLeft(symbolX);
    m_headSymbol->setRect(headSymbolRect);

    double y = headSymbolRect.bottom();
    m_axisSymbol->setLine(centerX, y, centerX, y + m_axisHeight);

    y = m_axisSymbol->boundingRect().bottom();
    m_endSymbol->setRect(symbolX, y, headSymbolRect.width(), END_SYMBOL_HEIGHT);

    if (oldHorizontalZenter != horizontalCenter()) {
        emit horizontalCenterChanged();
    }
    prepareGeometryChange();
    update();

    m_layoutDirty = false;
}

} // namespace msc
