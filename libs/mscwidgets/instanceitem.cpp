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

#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/instanceenditem.h"
#include "baseitems/instanceheaditem.h"
#include "baseitems/objectslinkitem.h"
#include "baseitems/textitem.h"
#include "commands/common/commandsstack.h"
#include "messageitem.h"
#include "mscinstance.h"

#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainter>

namespace msc {

InstanceItem::InstanceItem(msc::MscInstance *instance, QGraphicsItem *parent)
    : InteractiveObject(instance, parent)
    , m_instance(instance)
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_headSymbol(new InstanceHeadItem(this))
    , m_endSymbol(new InstanceEndItem(m_instance->explicitStop(), this))
{
    Q_ASSERT(m_instance != nullptr);

    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);
    connect(m_instance, &msc::MscInstance::kindChanged, this, &msc::InstanceItem::setKind);

    m_headSymbol->setName(m_instance->name());
    m_headSymbol->setKind(m_instance->kind());
    updateLayout();

    setFlags(ItemSendsGeometryChanges | ItemIsSelectable);

    // values are based on screenshot from https://git.vikingsoftware.com/esa/msceditor/issues/30
    QPen axisPen(Qt::darkGray);
    axisPen.setWidthF(3.);
    m_axisSymbol->setPen(axisPen);

    connect(m_headSymbol, &InstanceHeadItem::nameEdited, this, &InstanceItem::onNameEdited);
    connect(m_headSymbol, &InstanceHeadItem::kindEdited, this, &InstanceItem::onKindEdited);
    connect(m_headSymbol, &InstanceHeadItem::manualMoveRequested, this, [this](const QPointF &from, const QPointF &to) {
        if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Location::Center)) {
            onMoveRequested(gp, from, to);
        }
    });
    connect(m_headSymbol, &InstanceHeadItem::manualMoveFinished, this, &InstanceItem::moveLeftIfOverlaps);
    connect(m_headSymbol, &InstanceHeadItem::layoutUpdated, this, &InstanceItem::updateLayout);

    void();
}

MscInstance *InstanceItem::modelItem() const
{
    return m_instance;
}

QString InstanceItem::name() const
{
    return m_headSymbol->name();
}

QString InstanceItem::kind() const
{
    return m_headSymbol->kind();
}

void InstanceItem::setAxisHeight(qreal height)
{
    if (qFuzzyCompare(m_axisHeight, height)) {
        return;
    }
    m_axisHeight = height;
    rebuildLayout();
}

qreal InstanceItem::axisHeight() const
{
    return m_axisHeight;
}

QLineF InstanceItem::axis() const
{
    return m_axisSymbol->line().translated(pos());
}

void InstanceItem::setName(const QString &name)
{
    if (this->name() == name && m_instance->name() == name)
        return;

    m_instance->setName(name);
    m_headSymbol->setName(name);

    if (!moveLeftIfOverlaps())
        Q_EMIT needRelayout();
}

void InstanceItem::setKind(const QString &kind)
{
    if (this->kind() == kind && m_instance->kind() == kind)
        return;

    m_instance->setKind(kind);
    m_headSymbol->setKind(kind);

    if (!moveLeftIfOverlaps())
        Q_EMIT needRelayout();
}

void InstanceItem::rebuildLayout()
{
    m_boundingRect = QRectF();
    buildLayout();
}

void InstanceItem::buildLayout()
{
    prepareGeometryChange();

    QRectF headRect(m_headSymbol->boundingRect());
    const qreal endSymbolHeight = m_endSymbol->height();

    // precalculate own default size:
    if (m_boundingRect.isEmpty()) {
        m_boundingRect.setTopLeft(headRect.topLeft());
        m_boundingRect.setWidth(headRect.width());
        m_boundingRect.setHeight(headRect.height() + m_axisHeight + endSymbolHeight);
        updateGripPoints();
    }

    // move end symb to the bottom:
    const QRectF footerRect(m_boundingRect.left(), m_boundingRect.bottom() - endSymbolHeight, m_boundingRect.width(),
                            endSymbolHeight);
    m_endSymbol->setRect(footerRect);

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2 = m_endSymbol->isStop() ? footerRect.center() : QPointF(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));
}

void InstanceItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center) {
        const QPointF &delta = { (to - from).x(), 0. };
        setPos(pos() + delta);
    }
}

void InstanceItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

QPainterPath InstanceItem::shape() const
{
    QPainterPath result;
    result.addRect(m_headSymbol->boundingRect());
    result.addRect(m_endSymbol->boundingRect());
    result.addPath(ObjectsLinkItem::hoverableLine(m_axisSymbol->line()));
    return result;
}

void InstanceItem::setBoundingRect(const QRectF &geometry)
{
    if (geometry == boundingRect())
        return;

    prepareGeometryChange();
    m_boundingRect = geometry;
    if (m_gripPoints)
        m_gripPoints->updateLayout();
    updateLayout();
}

InstanceItem *InstanceItem::createDefaultItem(MscInstance *instance, const QPointF &pos)
{
    InstanceItem *messageItem = new InstanceItem(instance);
    messageItem->setPos(pos);

    return messageItem;
}

void InstanceItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });

    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &InstanceItem::onManualGeometryChangeFinished, Qt::UniqueConnection);

    const qreal zVal(m_gripPoints->zValue() - 1.);
    m_headSymbol->setZValue(zVal);
    m_axisSymbol->setZValue(zVal);
    m_endSymbol->setZValue(zVal);
}

void InstanceItem::onNameEdited(const QString &newName)
{
    if (newName.isEmpty()) {
        return;
    }

    using namespace msc::cmd;
    CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(this->modelItem()), newName });
}

void InstanceItem::onKindEdited(const QString &newKind)
{
    using namespace msc::cmd;
    CommandsStack::push(RenameInstanceKind, { QVariant::fromValue<MscEntity *>(this->modelItem()), newKind });
}

void InstanceItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &, const QPointF &)
{
    if (pos == GripPoint::Location::Center) {
        Q_EMIT moved(this);
    }
}

bool InstanceItem::moveLeftIfOverlaps()
{
    static constexpr qreal paddingPixels = { 40 };

    if (!scene())
        return false;

    const QRectF &mySceneRect(sceneBoundingRect());
    for (const InstanceItem *const other : utils::itemByPos<InstanceItem, QRectF>(scene(), mySceneRect)) {
        if (other == this)
            continue;
        const QRectF &intersection(other->sceneBoundingRect().intersected(mySceneRect));
        if (!intersection.isEmpty()) {
            QRectF mySceneRectValid(mySceneRect);
            mySceneRectValid.moveRight(intersection.left() - paddingPixels);

            const QPointF &delta(mySceneRectValid.center() - mySceneRect.center());
            moveBy(delta.x(), 0.); // TODO: use the CmdInstanceItemMove instead?

            return moveLeftIfOverlaps();
        }
    }

    Q_EMIT moved(this);
    return true;
}

} // namespace msc
