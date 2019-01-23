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
#include "baseitems/objectslinkitem.h"
#include "baseitems/grippoint.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/common/utils.h"
#include "baseitems/instanceenditem.h"
#include "commands/common/commandsstack.h"

#include <mscinstance.h>

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QApplication>
#include <QLinearGradient>

namespace msc {

static constexpr qreal SymbolWidth = { 60.0 };
const qreal InstanceItem::StartSymbolHeight = 20.;

QLinearGradient InstanceItem::createGradientForKind(const QGraphicsItem *itemKind)
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

QLinearGradient InstanceItem::createGradientForName(const QGraphicsItem *itemName)
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

InstanceItem::InstanceItem(msc::MscInstance *instance, QGraphicsItem *parent)
    : InteractiveObject(instance, parent)
    , m_instance(instance)
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_headSymbol(new QGraphicsRectItem(this))
    , m_nameItem(new TextItem(this))
    , m_kindItem(new TextItem(this))
    , m_endSymbol(new InstanceEndItem(m_instance->explicitStop(), this))
{
    Q_ASSERT(m_instance != nullptr);

    updateText(m_nameItem, m_instance->name());
    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);

    updateText(m_kindItem, m_instance->kind());
    connect(m_instance, &msc::MscInstance::kindChanged, this, &msc::InstanceItem::setKind);

    updateLayout();

    setFlags(QGraphicsItem::ItemSendsGeometryChanges);

    m_kindItem->setBackgroundColor(Qt::transparent);

    // values are based on screenshot from https://git.vikingsoftware.com/esa/msceditor/issues/30
    QPen axisPen(Qt::darkGray);
    axisPen.setWidthF(3.);
    m_axisSymbol->setPen(axisPen);

    m_nameItem->setEditable(true);
    m_kindItem->setEditable(true);
    connect(m_nameItem, &TextItem::edited, this, &InstanceItem::onNameEdited, Qt::QueuedConnection);
    connect(m_kindItem, &TextItem::edited, this, &InstanceItem::onKindEdited, Qt::QueuedConnection);
}

void InstanceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    updateText(m_nameItem, name);
}

void InstanceItem::setKind(const QString &kind)
{
    if (this->kind() == kind && m_instance->kind() == kind)
        return;

    m_instance->setKind(kind);
    updateText(m_kindItem, kind);
}

void InstanceItem::updateText(TextItem *holder, const QString &text)
{
    if (!holder)
        return;

    holder->setPlainText(text);
    holder->adjustSize();
    rebuildLayout();

    ensureNotOverlapped();

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

    QRectF nameRect({ 0., 0. }, m_nameItem->boundingRect().size());
    const QRectF kindRect(m_kindItem->boundingRect());
    QRectF kindR(nameRect.bottomLeft(),
                 QSizeF(qMax(kindRect.width(), qMax(nameRect.width(), SymbolWidth)),
                        qMax(kindRect.height(), StartSymbolHeight)));

    // precalculate own default size:
    if (m_boundingRect.isEmpty()) {
        m_boundingRect.setTopLeft(nameRect.topLeft());
        m_boundingRect.setWidth(qMax(nameRect.width(), kindR.width()));
        m_boundingRect.setHeight(nameRect.height() + kindR.height() + m_axisHeight + InstanceEndItem::EndSymbolHeight);
        updateGripPoints();
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
    m_kindItem->setTextWidth(m_boundingRect.width());
    m_nameItem->setTextWidth(m_boundingRect.width());

    QRectF headRect = m_kindItem->boundingRect().translated(m_kindItem->pos());
    headRect.setWidth(m_boundingRect.width());
    m_headSymbol->setRect(headRect);

    // move end symb to the bottom:
    QRectF footerRect(m_boundingRect.left(), m_boundingRect.bottom() - InstanceEndItem::EndSymbolHeight,
                      m_boundingRect.width(), InstanceEndItem::EndSymbolHeight);
    m_endSymbol->setRect(footerRect);

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));

    // update head gradient:
    m_nameItem->setBackgroundGradient(createGradientForName(m_nameItem));
    m_headSymbol->setBrush(createGradientForKind(m_headSymbol));

    m_layoutDirty = false;
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
    result.addRect(m_nameItem->boundingRect());
    result.addRect(m_kindItem->boundingRect());
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
            &InstanceItem::onManualGeometryChangeFinished);

    m_headSymbol->setZValue(m_gripPoints->zValue() - 1);
    m_nameItem->setZValue(m_gripPoints->zValue() - 1);
    m_kindItem->setZValue(m_gripPoints->zValue() - 1);
    m_axisSymbol->setZValue(m_gripPoints->zValue() - 1);
    m_endSymbol->setZValue(m_gripPoints->zValue() - 1);
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

void InstanceItem::ensureNotOverlapped()
{
    static constexpr qreal paddingPixels = { 40 };

    if (!scene())
        return;

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

            ensureNotOverlapped();
            return;
        }
    }
    Q_EMIT needRearrange();
}
} // namespace msc
