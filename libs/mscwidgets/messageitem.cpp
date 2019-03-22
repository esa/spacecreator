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

#include "messageitem.h"

#include "baseitems/arrowitem.h"
#include "baseitems/common/objectslink.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/labeledarrowitem.h"
#include "baseitems/msgidentificationitem.h"
#include "chartitem.h"
#include "commands/common/commandsstack.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPolygonF>
#include <QUndoStack>

namespace msc {

MessageItem::MessageItem(MscMessage *message, InstanceItem *source, InstanceItem *target, qreal y,
                         QGraphicsItem *parent)
    : InteractiveObject(message, parent)
    , m_message(message)
    , m_arrowItem(new LabeledArrowItem(this))
{
    Q_ASSERT(m_message != nullptr);

    connect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::updateDisplayText);
    updateDisplayText();

    connect(m_arrowItem, &LabeledArrowItem::layoutChanged, this, &MessageItem::commitGeometryChange);
    connect(m_arrowItem, &LabeledArrowItem::textEdited, this, &MessageItem::onRenamed);
    connect(m_arrowItem, &LabeledArrowItem::textChanged, this, &MessageItem::onTextChanged);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    connectObjects(source, target, y);

    m_arrowItem->setColor(QColor("#3e47e6")); // see https://git.vikingsoftware.com/esa/msceditor/issues/30
    m_arrowItem->setDashed(isCreator());
}

void MessageItem::onTextChanged()
{
    rebuildLayout();
    commitGeometryChange();

    if (m_gripPoints)
        m_gripPoints->updateLayout();

    update();
}

MscMessage *MessageItem::modelItem() const
{
    return m_message;
}

void MessageItem::connectObjects(InstanceItem *source, InstanceItem *target, qreal y)
{
    setPositionChangeIgnored(true); // ignore position changes in MessageItem::itemChange

    setY(y);
    if (source || target)
        setInstances(source, target);

    setPositionChangeIgnored(false);
}

void MessageItem::setInstances(InstanceItem *sourceInstance, InstanceItem *targetInstance)
{
    m_layoutDirty = true;
    setSourceInstanceItem(sourceInstance);
    setTargetInstanceItem(targetInstance);
    m_layoutDirty = false;
    rebuildLayout();
}

void MessageItem::setSourceInstanceItem(InstanceItem *sourceInstance)
{
    if (sourceInstance == m_sourceInstance) {
        return;
    }

    if (m_sourceInstance) {
        disconnect(m_sourceInstance, nullptr, this, nullptr);
    }

    m_sourceInstance = sourceInstance;
    if (m_sourceInstance) {
        connect(m_sourceInstance, &InteractiveObject::relocated, this, &MessageItem::onSourceInstanceMoved,
                Qt::DirectConnection);
        m_message->setSourceInstance(m_sourceInstance->modelItem());
    } else
        m_message->setSourceInstance(nullptr);

    updateTooltip();
}

void MessageItem::setTargetInstanceItem(InstanceItem *targetInstance)
{
    if (targetInstance == m_targetInstance) {
        return;
    }

    if (m_targetInstance) {
        disconnect(m_targetInstance, nullptr, this, nullptr);
    }

    m_targetInstance = targetInstance;
    if (m_targetInstance) {
        connect(m_targetInstance, &InteractiveObject::relocated, this, &MessageItem::onTargetInstanceMoved,
                Qt::DirectConnection);
        m_message->setTargetInstance(m_targetInstance->modelItem());
    } else
        m_message->setTargetInstance(nullptr);

    updateTooltip();
}

void MessageItem::updateTooltip()
{
    const QString env(tr("Env"));
    setToolTip(tr("%1: %2→%3")
                       .arg(name(), m_sourceInstance ? m_sourceInstance->name() : env,
                            m_targetInstance ? m_targetInstance->name() : env));
}

QString MessageItem::name() const
{
    return m_message->name();
}

void MessageItem::setName(const QString &name)
{
    m_message->setName(name);
    m_arrowItem->setText(m_message->name());

    updateLayout();
    Q_EMIT needRelayout();
}

QRectF MessageItem::boundingRect() const
{
    return m_arrowItem->boundingRect();
}

QPair<QPointF, bool> MessageItem::commentPoint() const
{
    if (head().x() > tail().x() && !m_targetInstance)
        return qMakePair(tail(), true);

    if (tail().x() > head().x() && !m_sourceInstance)
        return qMakePair(head(), true);

    return qMakePair(head().x() > tail().x() ? head() : tail(), false);
}

void MessageItem::rebuildLayout()
{
    // at first apply cif, if any
    using namespace cif;
    const QVector<CifBlockShared> &cifs = m_message->cifs();
    if (!cifs.isEmpty()) {
        QGraphicsScene *scene = this->scene();
        for (const CifBlockShared &cif : cifs) {
            if (cif->blockType() == CifLine::CifType::Message) {
                const QVector<QPoint> &cifPixels = cif->payload(CifLine::CifType::Message).value<QVector<QPoint>>();
                if (!cifPixels.isEmpty()) {
                    const QVector<QPointF> &sceneCoords = utils::cifToScene(cifPixels, scene);
                    m_arrowItem->arrow()->setTurnPoints(sceneCoords);
                }
            }
        }
    }

    auto itemCenterScene = [](InstanceItem *item) {
        QPointF res;
        if (item) {
            const QPointF b = item->boundingRect().center();
            res = item->mapToScene(b);
        }

        return res;
    };
    const QPointF fromC(itemCenterScene(m_sourceInstance).x(), y());
    const QPointF toC(itemCenterScene(m_targetInstance).x(), y());

    auto getChartBox = [this]() {
        // TODO: store the ChartItem* instance somewhere to avoid lookup whithin each MessageItem
        const QList<ChartItem *> boxes = utils::toplevelItems<ChartItem>(scene());
        return boxes.size() == 1 ? boxes.first()->sceneBoundingRect() : scene()->sceneRect();
    };

    const QRectF boxRect = getChartBox();
    auto extendToNearestEdge = [&boxRect](const QPointF &target) {
        const QLineF left({ boxRect.left(), target.y() }, target);
        const QLineF right({ boxRect.right(), target.y() }, target);

        if (left.length() <= right.length())
            return QPointF(boxRect.left(), target.y());
        else
            return QPointF(boxRect.right(), target.y());
    };

    QPointF pntFrom, pntTo;
    if (isCreator() && m_targetInstance) {
        // make the CREATE message point to the correct instance's "edge" and not its center
        pntFrom = fromC;
        pntTo.ry() = toC.y();
        const QRectF &targetR = m_targetInstance->boundingRect().translated(m_targetInstance->pos());
        pntTo.rx() = toC.x() > fromC.x() ? targetR.left() : targetR.right();
    } else if (m_sourceInstance && m_targetInstance) {
        // connect two centers (y)
        pntFrom = fromC;
        pntTo = toC;
    } else if (m_sourceInstance) {
        // instance to external
        pntFrom = fromC;
        pntTo = extendToNearestEdge(pntFrom);
    } else if (m_targetInstance) {
        // external to instance
        pntTo = toC;
        pntFrom = extendToNearestEdge(pntTo);
    }

    const bool bothAreNulls = pntFrom.isNull() && pntTo.isNull();
    if (!bothAreNulls) {
        const QPointF &linkCenterInScene =
                m_arrowItem->arrow()->makeArrow(m_sourceInstance, pntFrom, m_targetInstance, pntTo);
        setPositionChangeIgnored(true);
        setPos(linkCenterInScene);
        setPositionChangeIgnored(false);
    }

    commitGeometryChange();
}

QPainterPath MessageItem::shape() const
{
    QPainterPath result;
    result.addPath(m_arrowItem->shape());
    return result;
}

void MessageItem::updateGripPoints()
{
    if (m_gripPoints) {
        m_gripPoints->updateLayout();

        const QPointF &start(m_arrowItem->arrow()->anchorPointSource());
        const QPointF &end(m_arrowItem->arrow()->anchorPointTarget());

        m_gripPoints->setGripPointPos(GripPoint::Left, start);
        m_gripPoints->setGripPointPos(GripPoint::Right, end);
    }
}

QVariant MessageItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged: {
        if (proceedPositionChange()) {
            updateSourceAndTarget(value.toPointF() - m_prevPos);
        }
        break;
    }
    default:
        break;
    }
    return InteractiveObject::itemChange(change, value);
}

QString MessageItem::displayTextFromModel() const
{
    if (m_message->parameters().isEmpty())
        return m_message->name();

    QString parameters;
    for (const auto &param : m_message->parameters()) {
        if (!parameters.isEmpty())
            parameters += ", ";
        parameters += param.parameter();
    }
    return m_message->name() + "(" + parameters + ")";
}

bool MessageItem::updateSourceAndTarget(const QPointF &shift)
{
    bool res(false);
    const InstanceItem *prevSource(m_sourceInstance);
    const InstanceItem *prevTarget(m_targetInstance);

    ArrowItem *arrow = m_arrowItem->arrow();

    const QPointF shiftedSource(arrow->anchorPointSource() + shift);
    res |= updateSource(shiftedSource, ObjectAnchor::Snap::NoSnap);
    const QPointF shiftedTarget(arrow->anchorPointTarget() + shift);
    res |= updateTarget(shiftedTarget, ObjectAnchor::Snap::NoSnap);

    if (!isAutoResizable())
        return res;

    // snap to the instance's center:
    const bool sourceFound(!prevSource && m_sourceInstance);
    const bool targetFound(!prevTarget && m_targetInstance);

    if (sourceFound) {
        const QPointF &instanceCenterScene(m_sourceInstance->sceneBoundingRect().center());
        const QPointF newAnchor(instanceCenterScene.x(), shiftedSource.y());
        const QPointF &delta(newAnchor - arrow->link()->source()->point());

        const bool updated =
                updateSource(newAnchor, isAutoResizable() ? ObjectAnchor::Snap::SnapTo : ObjectAnchor::Snap::NoSnap);

        if (updated && !m_targetInstance && !delta.isNull())
            updateTarget(arrow->link()->target()->point() + delta, ObjectAnchor::Snap::NoSnap);
    }
    if (targetFound) {
        const QPointF &instanceCenterScene(m_targetInstance->sceneBoundingRect().center());
        const QPointF newAnchor(instanceCenterScene.x(), shiftedTarget.y());
        const QPointF &delta(newAnchor - arrow->link()->target()->point());

        const bool updated =
                updateTarget(newAnchor, isAutoResizable() ? ObjectAnchor::Snap::SnapTo : ObjectAnchor::Snap::NoSnap);

        if (updated && !m_sourceInstance && !delta.isNull())
            updateSource(arrow->link()->source()->point() + delta, ObjectAnchor::Snap::NoSnap);
    }

    return res;
}

bool MessageItem::updateSource(const QPointF &to, ObjectAnchor::Snap snap)
{
    setSourceInstanceItem(hoveredItem(to));
    const bool res = m_arrowItem->updateSource(m_sourceInstance, to, snap);
    updateGripPoints();
    commitGeometryChange();
    return res;
}

bool MessageItem::updateTarget(const QPointF &to, ObjectAnchor::Snap snap)
{
    setTargetInstanceItem(hoveredItem(to));
    const bool res = m_arrowItem->updateTarget(m_targetInstance, to, snap);
    updateGripPoints();
    commitGeometryChange();
    return res;
}

InstanceItem *MessageItem::hoveredItem(const QPointF &hoverPoint) const
{
    const QVector<InstanceItem *> &others = utils::instanceItemsByPos(scene(), hoverPoint);
    return others.isEmpty() ? nullptr : others.first();
}

void MessageItem::commitGeometryChange()
{
    prepareGeometryChange();
    m_boundingRect = m_arrowItem->boundingRect();
}

void MessageItem::onSourceInstanceMoved(const QPointF &from, const QPointF &to)
{
    setPositionChangeIgnored(true);
    const QPointF offset(to - from);

    const QPointF srcPoint(m_arrowItem->arrow()->anchorPointSource() + offset);
    QPointF dstPoint(m_arrowItem->arrow()->anchorPointTarget());

    if (!m_targetInstance) {
        moveBy(offset.x(), offset.y());
        dstPoint += offset;
    }

    m_arrowItem->updatePoints(srcPoint, dstPoint);
    setPositionChangeIgnored(false);

    updateLayout();
}

void MessageItem::onTargetInstanceMoved(const QPointF &from, const QPointF &to)
{
    setPositionChangeIgnored(true);
    const QPointF offset(to - from);

    QPointF srcPoint(m_arrowItem->arrow()->anchorPointSource());
    const QPointF dstPoint(m_arrowItem->arrow()->anchorPointTarget() + offset);

    if (!m_sourceInstance) {
        moveBy(offset.x(), offset.y());
        srcPoint += offset;
    }

    m_arrowItem->updatePoints(srcPoint, dstPoint);
    setPositionChangeIgnored(false);

    updateLayout();
}

QPointF MessageItem::head() const
{
    return m_arrowItem->mapToScene(m_arrowItem->endSignPos());
}

void MessageItem::setHead(const QPointF &head, ObjectAnchor::Snap snap)
{
    if (head == this->head() && snap == ObjectAnchor::Snap::NoSnap)
        return;

    updateTarget(head, snap);
}

QPointF MessageItem::tail() const
{
    return m_arrowItem->mapToScene(m_arrowItem->startSignPos());
}

void MessageItem::setTail(const QPointF &tail, ObjectAnchor::Snap snap)
{
    if (tail == this->tail() && snap == ObjectAnchor::Snap::NoSnap)
        return;

    updateSource(tail, snap);
}

bool MessageItem::isAutoResizable() const
{
    return m_autoResize;
}

void MessageItem::setAutoResizable(bool resizable)
{
    if (resizable == m_autoResize)
        return;

    m_autoResize = resizable;
}

void MessageItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void MessageItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    if (isCreator())
        return;

    //    const QPointF shift(to - from);
    if (gp->location() == GripPoint::Left) {
        //        setTail(tail() + shift, ObjectAnchor::Snap::NoSnap);
        //        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
        //                                      { QVariant::fromValue<MessageItem *>(this), head(), tail() + shift });
        m_arrowItem->updateSource(m_sourceInstance, to, ObjectAnchor::Snap::NoSnap);
    } else if (gp->location() == GripPoint::Right) {
        //        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
        //                                      { QVariant::fromValue<MessageItem *>(this), head() + shift, tail() });
        //        setHead(head() + shift, ObjectAnchor::Snap::NoSnap);
        m_arrowItem->updateTarget(m_targetInstance, to, ObjectAnchor::Snap::NoSnap);
    }
    updateGripPoints();
}

MessageItem *MessageItem::createDefaultItem(MscMessage *message, const QPointF &pos)
{
    MessageItem *messageItem = new MessageItem(message, nullptr, nullptr, pos.y());
    static constexpr qreal halfLength(ArrowItem::DEFAULT_WIDTH / 2.);
    const QPointF head(halfLength, pos.y());
    const QPointF tail(-halfLength, pos.y());
    messageItem->setHead(head, ObjectAnchor::Snap::NoSnap);
    messageItem->setTail(tail, ObjectAnchor::Snap::NoSnap);
    messageItem->setPos(pos);

    return messageItem;
}

void MessageItem::performSnap()
{
    setHead(head(), ObjectAnchor::Snap::SnapTo);
    setTail(tail(), ObjectAnchor::Snap::SnapTo);
}

bool MessageItem::ignorePositionChange() const
{
    return m_posChangeIgnored;
}

bool MessageItem::proceedPositionChange() const
{
    return !ignorePositionChange();
}

void MessageItem::setPositionChangeIgnored(bool ignored)
{
    m_posChangeIgnored = ignored;
}

void MessageItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_arrowItem->enableEditMode();
}

bool MessageItem::isCreator() const
{
    return m_message && m_message->messageType() == MscMessage::MessageType::Create;
}

void MessageItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();

    if (isCreator())
        m_gripPoints->setUsedPoints(GripPoint::Locations());
    else
        m_gripPoints->setUsedPoints({ GripPoint::Location::Left, GripPoint::Location::Right });

    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &MessageItem::onManualGeometryChangeFinished, Qt::UniqueConnection);

    m_arrowItem->setZValue(m_gripPoints->zValue() - 1);
}

void MessageItem::onRenamed(const QString &title)
{
    if (title.isEmpty()) {
        return;
    }

    if (m_preventRecursion) {
        qWarning() << "Unexpected signal slot recursion for message:" << title;
        Q_ASSERT(m_preventRecursion);
        return;
    }
    m_preventRecursion = true;

    const QString name = MsgIdentificationItem::nameFromText(title);
    const msc::MscParameterList parameters = MsgIdentificationItem::parametersFromText(title);

    using namespace msc::cmd;

    // prevent recursion, as parametes ans name are not set as one unit
    disconnect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::updateDisplayText);
    CommandsStack::current()->beginMacro(tr("Set message identification"));
    CommandsStack::push(
            SetParameterList,
            { QVariant::fromValue<MscEntity *>(m_message), QVariant::fromValue<MscParameterList>(parameters) });
    CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(m_message), name });
    CommandsStack::current()->endMacro();
    connect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::updateDisplayText);

    m_preventRecursion = false;
}

void MessageItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &, const QPointF &to)
{
    if (pos == GripPoint::Left) {
        Q_EMIT retargeted(this, to, msc::MscMessage::EndType::SOURCE_TAIL);
    } else if (pos == GripPoint::Right) {
        Q_EMIT retargeted(this, to, msc::MscMessage::EndType::TARGET_HEAD);
    }
}

void MessageItem::updateDisplayText()
{
    QString modelText = displayTextFromModel();
    if (modelText != displayedText()) {
        m_arrowItem->setText(modelText);

        updateLayout();
        Q_EMIT needRelayout();
    }
}

void MessageItem::addMessagePoint(const QPointF &scenePoint)
{
    m_arrowItem->arrow()->addTurnPoint(scenePoint);
}

QVector<QPointF> MessageItem::messagePoints() const
{
    return m_arrowItem->arrow()->turnPoints();
}

void MessageItem::applyCif()
{
    QSignalBlocker sb(this);
    setPositionChangeIgnored(true);
    using namespace cif;
    QGraphicsScene *scene = this->scene();
    const QVector<CifBlockShared> &cifs = modelEntity()->cifs();
    for (const CifBlockShared &cif : cifs) {
        switch (cif->blockType()) {
        case CifLine::CifType::Message: {
            const QVector<QPoint> &points = cif->payload(CifLine::CifType::Message).value<QVector<QPoint>>();
            m_arrowItem->arrow()->setTurnPoints(utils::cifToScene(points, scene));
            break;
        }
        default: {
#ifdef QT_DEBUG
            qWarning() << Q_FUNC_INFO << "Unsupported cif:" << cif->blockType();
#endif
            break;
        }
        }
    }
    setPositionChangeIgnored(false);
}

QString MessageItem::displayedText() const
{
    return m_arrowItem->text();
}

} // namespace msc
