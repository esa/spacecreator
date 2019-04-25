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
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/objectslink.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/labeledarrowitem.h"
#include "baseitems/msgidentificationitem.h"
#include "chartitem.h"
#include "cif/cifblockfactory.h"
#include "cif/cifblocks.h"
#include "cif/ciflines.h"
#include "commands/common/commandsstack.h"
#include "messagedialog.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPolygonF>
#include <QUndoStack>

namespace msc {

MessageItem::GeometryNotificationBlocker ::GeometryNotificationBlocker(MessageItem *target)
    : m_target(target)
    , m_storedPositionChangeIgnored(target->ignorePositionChange())
{
    m_target->setPositionChangeIgnored(true);
}

MessageItem::GeometryNotificationBlocker ::~GeometryNotificationBlocker()
{
    if (m_target) {
        m_target->setPositionChangeIgnored(m_storedPositionChangeIgnored);
    }
}

MessageItem::MessageItem(MscMessage *message, InstanceItem *source, InstanceItem *target, QGraphicsItem *parent)
    : InteractiveObject(message, parent)
    , m_message(message)
    , m_arrowItem(new LabeledArrowItem(this))
{
    Q_ASSERT(m_message != nullptr);

    connect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::updateDisplayText);
    updateDisplayText();

    connect(m_arrowItem, &LabeledArrowItem::textEdited, this, &MessageItem::onRenamed);
    connect(m_arrowItem, &LabeledArrowItem::textChanged, this, &MessageItem::onTextChanged);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    setInstances(source, target);

    m_arrowItem->setColor(QColor("#3e47e6")); // see https://git.vikingsoftware.com/esa/msceditor/issues/30
    m_arrowItem->setDashed(isCreator());
}

void MessageItem::onTextChanged()
{
    rebuildLayout();

    if (m_gripPoints)
        m_gripPoints->updateLayout();

    update();
}

void MessageItem::showMessageDialog()
{
    if (m_message->messageType() == MscMessage::MessageType::Message) {
        MessageDialog dialog(m_message);
        dialog.exec();
    }
}

MscMessage *MessageItem::modelItem() const
{
    return m_message;
}

void MessageItem::setInstances(InstanceItem *sourceInstance, InstanceItem *targetInstance)
{
    m_layoutDirty = true;
    const bool sourceChanged = setSourceInstanceItem(sourceInstance);
    const bool targetChanged = setTargetInstanceItem(targetInstance);

    if (sourceChanged || targetChanged) {
        rebuildLayout();
    }

    m_layoutDirty = false;
}

bool MessageItem::setSourceInstanceItem(InstanceItem *sourceInstance)
{
    if (m_targetInstance == sourceInstance)
        return false;
    if (sourceInstance == m_sourceInstance)
        return false;

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
    return true;
}

bool MessageItem::setTargetInstanceItem(InstanceItem *targetInstance)
{
    if (m_sourceInstance == targetInstance)
        return false;
    if (targetInstance == m_targetInstance)
        return false;

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
    return true;
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

void MessageItem::postCreatePolishing()
{
    showMessageDialog();
    if (m_message->messageType() == MscMessage::MessageType::Create)
        m_arrowItem->enableEditMode();
}

void MessageItem::rebuildLayout()
{
    const QVector<QPointF> &points = messagePoints();
    if (points.size() < 2)
        return;

    const bool wasGlobal = modelItem()->isGlobal();

    updateSource(points.first(), ObjectAnchor::Snap::NoSnap, m_sourceInstance);
    updateTarget(points.last(), ObjectAnchor::Snap::NoSnap, m_targetInstance);

    // ensure that ENV has not been replaced by the edge of an instance located nearby
    if (wasGlobal && modelItem()->isGlobal() != wasGlobal) {
        QSignalBlocker suppressDataChanged(m_message);
        if (m_sourceInstance && !qFuzzyCompare(1. + points.first().x(), 1 + m_sourceInstance->centerInScene().x()))
            setSourceInstanceItem(nullptr);

        if (m_targetInstance && !qFuzzyCompare(1. + points.last().x(), 1 + m_targetInstance->centerInScene().x()))
            setTargetInstanceItem(nullptr);
    }

    if (!geometryManagedByCif())
        onChartBoxChanged(); // extends global message to chart edges, if necessary
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
    const InstanceItem *prevSource(m_sourceInstance);
    const InstanceItem *prevTarget(m_targetInstance);

    ArrowItem *arrow = m_arrowItem->arrow();

    const QPointF &shiftedSource = arrow->anchorPointSource() + shift;
    bool res = updateSource(shiftedSource, ObjectAnchor::Snap::NoSnap);
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

bool MessageItem::updateSource(const QPointF &to, ObjectAnchor::Snap snap, InstanceItem *keepInstance)
{
    setSourceInstanceItem(keepInstance ? keepInstance : hoveredItem(to));
    const bool res = m_arrowItem->updateSource(m_sourceInstance, to, snap);
    if (res) {
        updateMessagePoints();
        updateGripPoints();
        commitGeometryChange();
    }
    return res;
}

bool MessageItem::updateTarget(const QPointF &to, ObjectAnchor::Snap snap, InstanceItem *keepInstance)
{
    setTargetInstanceItem(keepInstance ? keepInstance : hoveredItem(to));
    const bool res = m_arrowItem->updateTarget(m_targetInstance, to, snap);

    if (res) {
        updateMessagePoints();
        updateGripPoints();
        commitGeometryChange();
    }

    return res;
}

InstanceItem *MessageItem::hoveredItem(const QPointF &hoverPoint) const
{
    const QVector<InstanceItem *> &others = utils::instanceItemsByPos(scene(), hoverPoint);
    return others.isEmpty() ? nullptr : others.first();
}

void MessageItem::commitGeometryChange()
{
    const QRectF &newBounds = mapRectFromItem(m_arrowItem, m_arrowItem->boundingRect());
    if (m_boundingRect != newBounds) {
        prepareGeometryChange();
        m_boundingRect = newBounds;
    }
}

void MessageItem::onSourceInstanceMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    const QPointF &desitnation = m_arrowItem->arrow()->anchorPointSource() + offset;
    updateSource(desitnation, ObjectAnchor::Snap::NoSnap, m_sourceInstance);

    scheduleLayoutUpdate();

    if (geometryManagedByCif())
        updateCif();
}

void MessageItem::onTargetInstanceMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    const QPointF &desitnation = m_arrowItem->arrow()->anchorPointTarget() + offset;
    updateTarget(desitnation, ObjectAnchor::Snap::NoSnap, m_targetInstance);

    scheduleLayoutUpdate();

    if (geometryManagedByCif())
        updateCif();
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
    if (from == to)
        return;

    if (isCreator())
        return;

    if (gp->location() == GripPoint::Left)
        updateSource(to, ObjectAnchor::Snap::NoSnap);
    else if (gp->location() == GripPoint::Right)
        updateTarget(to, ObjectAnchor::Snap::NoSnap);
}

void MessageItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &, const QPointF &to)
{
    // @todo use a command to support undo
    auto commitUpdate = [&](msc::MscMessage::EndType endType) {
        if (endType == msc::MscMessage::EndType::SOURCE_TAIL)
            updateSource(to, ObjectAnchor::Snap::SnapTo);
        else
            updateTarget(to, ObjectAnchor::Snap::SnapTo);

        if (utils::isHorizontal(messagePoints())) {
            if (geometryManagedByCif()) {
                m_message->clearCifs();
            }
        } else
            updateCif();
        Q_EMIT retargeted(this, to, endType);
    };

    commitUpdate(pos == GripPoint::Left ? msc::MscMessage::EndType::SOURCE_TAIL
                                        : msc::MscMessage::EndType::TARGET_HEAD);
}

MessageItem *MessageItem::createDefaultItem(MscMessage *message, const QPointF &pos)
{
    MessageItem *messageItem = new MessageItem(message);
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
    showMessageDialog();
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

void MessageItem::updateDisplayText()
{
    QString modelText = displayTextFromModel();
    if (modelText != displayedText()) {
        m_arrowItem->setText(modelText);

        scheduleLayoutUpdate();
    }
}

void MessageItem::addMessagePoint(const QPointF &scenePoint)
{
    m_arrowItem->arrow()->addTurnPoint(scenePoint);
}

QVector<QPointF> MessageItem::messagePoints() const
{
    const QVector<QPointF> &localPoints = m_arrowItem->arrow()->turnPoints();
    QVector<QPointF> scenePoints(localPoints.size());
    ArrowItem *pArrow = m_arrowItem->arrow();
    std::transform(localPoints.cbegin(), localPoints.cend(), scenePoints.begin(),
                   [&pArrow](const QPointF &point) { return pArrow->mapToScene(point); });

    return scenePoints;
}

cif::CifBlockShared MessageItem::mainCifBlock() const
{
    return cifBlockByType(mainCifType());
}

cif::CifBlockShared MessageItem::positionCifBlock() const
{
    return cifBlockByType(cif::CifLine::CifType::Position);
}

bool MessageItem::setMessagePointsNoCif(const QVector<QPointF> &scenePoints)
{
    const QVector<QPointF> &arrowPoints = messagePoints();
    if (scenePoints.size() < 2 || scenePoints == arrowPoints)
        return false;

    Q_ASSERT(!scenePoints.isEmpty());

    GeometryNotificationBlocker keepSilent(this);
    setPos(utils::lineCenter({ scenePoints.first(), scenePoints.last() }));
    m_arrowItem->arrow()->makeArrow(m_sourceInstance, scenePoints.first(), m_targetInstance, scenePoints.last());
    m_arrowItem->arrow()->setTurnPoints(scenePoints);

    commitGeometryChange();
    return true;
}

void MessageItem::setMessagePoints(const QVector<QPointF> &scenePoints, utils::CifUpdatePolicy cifUpdate)
{
    if (!setMessagePointsNoCif(scenePoints))
        return;

    switch (cifUpdate) {
    case utils::CifUpdatePolicy::DontChange:
        return;
    case utils::CifUpdatePolicy::UpdateIfExists: {
        if (!geometryManagedByCif())
            return;
        break;
    }
    default:
        break;
    }

    updateCif();
}

void MessageItem::applyCif()
{
    QSignalBlocker sb(this);
    GeometryNotificationBlocker geometryNotificationBlocker(this);
    if (const cif::CifBlockShared &cifBlock = mainCifBlock()) {
        const QVector<QPoint> &pointsCif = cifBlock->payload(mainCifType()).value<QVector<QPoint>>();
        bool converted(false);
        const QVector<QPointF> &pointsScene = utils::CoordinatesConverter::cifToScene(pointsCif, &converted);
        setMessagePoints(pointsScene, utils::CifUpdatePolicy::UpdateIfExists);
    }
}

QString MessageItem::displayedText() const
{
    return m_arrowItem->text();
}

void MessageItem::updateMessagePoints()
{
    QVector<QPointF> points = messagePoints();
    const int pointsCount = points.size();
    if (pointsCount < 2)
        return;

    bool updated(false);
    const QPointF &currentAnchorFrom = m_arrowItem->arrow()->anchorPointSource();
    if (points.first() != currentAnchorFrom) {
        points.replace(0, currentAnchorFrom);
        updated = true;
    }

    const QPointF &currentAnchorTo = m_arrowItem->arrow()->anchorPointTarget();
    if (points.last() != currentAnchorTo) {
        points.replace(pointsCount - 1, currentAnchorTo);
        updated = true;
    }

    if (updated)
        setMessagePoints(points, utils::CifUpdatePolicy::ForceCreate);
}

cif::CifLine::CifType MessageItem::mainCifType() const
{
    return isCreator() ? cif::CifLine::CifType::Create : cif::CifLine::CifType::Message;
}

void MessageItem::updateCif()
{
    using namespace cif;

    const cif::CifLine::CifType usedCifType = mainCifType();
    const QVector<QPoint> &pointsCif = utils::CoordinatesConverter::sceneToCif(messagePoints());
    if (!geometryManagedByCif()) {
        const CifBlockShared &emptyCif =
                isCreator() ? CifBlockFactory::createBlockCreate() : CifBlockFactory::createBlockMessage();
        m_message->addCif(emptyCif);
    }

    const CifBlockShared &msgCif = mainCifBlock();
    if (!msgCif->hasPayloadFor(usedCifType))
        msgCif->addLine(isCreator() ? CifLineShared(new CifLineCreate()) : CifLineShared(new CifLineMessage()));

    const QVector<QPoint> &pointsCifStored = msgCif->payload(mainCifType()).value<QVector<QPoint>>();
    if (pointsCifStored != pointsCif)
        msgCif->setPayload(QVariant::fromValue(pointsCif), usedCifType);
}

void MessageItem::moveSilentlyBy(const QPointF &shift)
{
    GeometryNotificationBlocker silently(this);
    InteractiveObject::moveSilentlyBy(shift);
}

void MessageItem::extendGlobalMessage()
{
    auto getChartBox = [this]() {
        if (ChartItem *chartItem = utils::CoordinatesConverter::currentChartItem())
            return chartItem->box();
        if (QGraphicsScene *scene = this->scene())
            return scene->sceneRect();
        return QRectF();
    };

    const QRectF boxRect = getChartBox();
    auto extendToNearestEdge = [&](const QPointF &shiftMe) {
        const QLineF left({ boxRect.left(), shiftMe.y() }, shiftMe);
        const QLineF right({ boxRect.right(), shiftMe.y() }, shiftMe);

        if (left.length() <= right.length())
            return QPointF(boxRect.left(), shiftMe.y());
        else
            return QPointF(boxRect.right(), shiftMe.y());
    };

    QVector<QPointF> points(messagePoints());
    const bool isFromEnv = !m_sourceInstance;
    const int shiftPointId = isFromEnv ? 0 : points.size() - 1;
    QPointF shiftMe(points.at(shiftPointId));
    if (!geometryManagedByCif()) {
        const QPointF &instanceCenter =
                isFromEnv ? m_targetInstance->centerInScene() : m_sourceInstance->centerInScene();
        shiftMe.rx() = instanceCenter.x();
    }
    const QPointF &shiftedMe = extendToNearestEdge(shiftMe);
    if (shiftedMe != points.at(shiftPointId)) {
        points.replace(shiftPointId, shiftedMe);
        setMessagePoints(points, utils::CifUpdatePolicy::UpdateIfExists);
    }
}

void MessageItem::onChartBoxChanged()
{
    if (!m_message->isGlobal())
        return;

    extendGlobalMessage();
}

} // namespace msc
