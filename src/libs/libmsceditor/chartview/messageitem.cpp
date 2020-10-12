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

#include "aadlchecks.h"
#include "baseitems/arrowitem.h"
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/common/objectslink.h"
#include "baseitems/labeledarrowitem.h"
#include "baseitems/msgidentificationitem.h"
#include "cif/cifblockfactory.h"
#include "cif/cifblocks.h"
#include "cif/ciflines.h"
#include "commands/common/commandsstack.h"
#include "commentitem.h"
#include "messagedialog.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "ui/grippointshandler.h"

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

MessageItem::MessageItem(MscMessage *message, ChartLayoutManager *chartLayoutManager, InstanceItem *source,
        InstanceItem *target, QGraphicsItem *parent)
    : InteractiveObject(message, parent)
    , m_message(message)
    , m_arrowItem(new LabeledArrowItem(this))
    , m_chartLayoutManager(chartLayoutManager)
{
    Q_ASSERT(m_message != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    connect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::checkAadlConnection);
    connect(m_message, &msc::MscMessage::dataChanged, this, &msc::MessageItem::updateDisplayText);
    updateDisplayText();

    connect(m_arrowItem, &LabeledArrowItem::textEdited, this, &MessageItem::onRenamed);
    connect(m_arrowItem, &LabeledArrowItem::textChanged, this, &MessageItem::onTextChanged);

    connect(m_message, &msc::MscMessage::sourceChanged, this, [&](msc::MscInstance *mscInstance) {
        if (!m_chartLayoutManager)
            return;
        msc::InstanceItem *instance = m_chartLayoutManager->itemForInstance(mscInstance);
        this->setSourceInstanceItem(instance);

        if (geometryManagedByCif())
            updateCif();
    });
    connect(m_message, &msc::MscMessage::targetChanged, this, [&](msc::MscInstance *mscInstance) {
        if (!m_chartLayoutManager)
            return;
        msc::InstanceItem *instance = m_chartLayoutManager->itemForInstance(mscInstance);
        this->setTargetInstanceItem(instance);

        if (geometryManagedByCif())
            updateCif();
    });
    connect(m_message, &MscMessage::cifPointsChanged, this, [this]() {
        applyCif();
        scheduleLayoutUpdate();
    });
    setInstances(source, target);

    m_arrowItem->setColor(MessageColor);
    m_arrowItem->setDashed(isCreator());
    checkAadlConnection();

    connect(this, &InteractiveObjectBase::relocated, [this](const QPointF &from, const QPointF &to) {
        if (proceedPositionChange()) {
            updateSourceAndTarget(to - from);
        }
    });

    if (m_chartLayoutManager && m_chartLayoutManager->aadlChecker()) {
        connect(m_chartLayoutManager->aadlChecker(), &msc::AadlChecks::ivCoreChanged, this,
                &msc::MessageItem::checkAadlConnection);
    }

    if (!isCreator()) {
        m_arrowItem->setEditable(false);
    }
    connect(m_arrowItem, &msc::LabeledArrowItem::doubleClicked, this, &msc::MessageItem::showMessageDialog);
}

void MessageItem::onTextChanged()
{
    rebuildLayout();

    if (auto gph = gripPointsHandler()) {
        gph->updateLayout();
    }

    checkAadlConnection();
    update();
}

void MessageItem::showMessageDialog()
{
    if (m_message->messageType() == MscMessage::MessageType::Message) {
        MessageDialog dialog(m_message);
        dialog.setAadlChecker(m_chartLayoutManager->aadlChecker());
        dialog.exec();
    }
}

MscMessage *MessageItem::modelItem() const
{
    return m_message;
}

void MessageItem::setInstances(InstanceItem *sourceInstance, InstanceItem *targetInstance)
{
    const bool sourceChanged = setSourceInstanceItem(sourceInstance);
    const bool targetChanged = setTargetInstanceItem(targetInstance);

    if (sourceChanged || targetChanged) {
        rebuildLayout();
    }

    instantLayoutUpdate();
}

bool MessageItem::setSourceInstanceItem(InstanceItem *sourceInstance)
{
    if (sourceInstance == m_sourceInstance || (sourceInstance && sourceInstance == m_targetInstance))
        return false;

    if (m_sourceInstance) {
        disconnect(m_sourceInstance, nullptr, this, nullptr);
    }

    m_sourceInstance = sourceInstance;
    if (m_sourceInstance) {
        connect(m_sourceInstance, &InteractiveObject::relocated, this, &MessageItem::onSourceInstanceMoved,
                Qt::DirectConnection);
    }

    QVector<QPointF> points = messagePoints();
    if (messagePoints().size() > 1) {
        QPointF &pt = points.first();
        if (m_sourceInstance)
            pt.setX(m_sourceInstance->sceneBoundingRect().center().x());
        else
            pt.setX(0);
        setMessagePoints(points);
    }

    updateTooltip();
    scheduleLayoutUpdate();
    Q_EMIT needUpdateLayout();
    return true;
}

InstanceItem *MessageItem::sourceInstanceItem() const
{
    return m_sourceInstance;
}

bool MessageItem::setTargetInstanceItem(InstanceItem *targetInstance)
{
    if (targetInstance == m_targetInstance || (targetInstance && targetInstance == m_sourceInstance))
        return false;

    if (m_targetInstance) {
        disconnect(m_targetInstance, nullptr, this, nullptr);
    }

    m_targetInstance = targetInstance;
    if (m_targetInstance) {
        connect(m_targetInstance, &InteractiveObject::relocated, this, &MessageItem::onTargetInstanceMoved,
                Qt::DirectConnection);
    }

    applyTargetPointFromInstance();
    updateTooltip();
    scheduleLayoutUpdate();
    Q_EMIT needUpdateLayout();
    return true;
}

InstanceItem *MessageItem::targetInstanceItem() const
{
    return m_targetInstance;
}

void MessageItem::updateTooltip()
{
    const QString env(tr("Env"));
    QString aadlMessage;
    if (!aadlConnectionOk()) {
        aadlMessage = "\n\n" + tr("This message does not have a corresponding AADL connection in the aadl model.");
    }
    setToolTip(tr("%1: %2→%3%4")
                       .arg(name(), m_sourceInstance ? m_sourceInstance->name() : env,
                               m_targetInstance ? m_targetInstance->name() : env, aadlMessage));
}

QString MessageItem::name() const
{
    return m_message->name();
}

QRectF MessageItem::boundingRect() const
{
    return m_arrowItem->boundingRect();
}

QRectF MessageItem::textBoundingRect() const
{
    return m_arrowItem->textBoundingRect();
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
    if (isCreator())
        m_arrowItem->enableEditMode();
}

void MessageItem::rebuildLayout()
{
    const QVector<QPointF> &points = messagePoints();
    if (points.size() < 2)
        return;

    const bool wasGlobal = wannabeGlobal();

    if (points.first() != tail())
        updateSource(points.first(), ObjectAnchor::Snap::NoSnap, m_sourceInstance);
    if (points.last() != head())
        updateTarget(points.last(), ObjectAnchor::Snap::NoSnap, m_targetInstance);

    if (wasGlobal && wannabeGlobal() != wasGlobal) {
        QSignalBlocker suppressDataChanged(m_message);
        if (m_sourceInstance && !qFuzzyCompare(1. + points.first().x(), 1 + m_sourceInstance->centerInScene().x()))
            setSourceInstanceItem(nullptr);

        if (m_targetInstance && !qFuzzyCompare(1. + points.last().x(), 1 + m_targetInstance->centerInScene().x()))
            setTargetInstanceItem(nullptr);
    }

    if (wannabeGlobal()) {
        extendGlobalMessage();
    }

    update();
}

QPainterPath MessageItem::shape() const
{
    return m_arrowItem->shape();
}

void MessageItem::updateGripPoints()
{
    InteractiveObjectBase::updateGripPoints();

    if (auto gph = gripPointsHandler()) {
        const QPointF &start(m_arrowItem->arrow()->anchorPointSource());
        const QPointF &end(m_arrowItem->arrow()->anchorPointTarget());

        gph->setGripPointPos(shared::ui::GripPoint::Left, start);
        gph->setGripPointPos(shared::ui::GripPoint::Right, end);
    }
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

    const QString &text = QString("%1(%2)").arg(m_message->name(), parameters);
    return text;
}

bool MessageItem::updateSourceAndTarget(const QPointF &shift)
{
    const InstanceItem *prevSource(m_sourceInstance);
    const InstanceItem *prevTarget(m_targetInstance);

    ArrowItem *arrow = m_arrowItem->arrow();

    const QPointF shiftedSource { arrow->anchorPointSource() + shift };
    const QPointF shiftedTarget { arrow->anchorPointTarget() + shift };
    bool res = updateSource(shiftedSource, ObjectAnchor::Snap::SnapTo, m_sourceInstance);
    res |= updateTarget(shiftedTarget, ObjectAnchor::Snap::SnapTo, m_targetInstance);

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
    if (keepInstance == nullptr && snap == ObjectAnchor::Snap::SnapTo)
        keepInstance = hoveredInstance(to);
    setSourceInstanceItem(keepInstance);
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
    if (keepInstance == nullptr && snap == ObjectAnchor::Snap::SnapTo) {
        static const bool extendToTarget = true;
        keepInstance = hoveredInstance(to, extendToTarget);
    }
    setTargetInstanceItem(keepInstance);
    bool res = true;
    if (isCreator()) {
        res = m_arrowItem->updateTarget(m_targetInstance, to, ObjectAnchor::Snap::NoSnap);
    } else {
        res = m_arrowItem->updateTarget(m_targetInstance, to, snap);
    }
    if (res) {
        updateMessagePoints();
        updateGripPoints();
        commitGeometryChange();
    }

    return res;
}

/*!
   Returns the instance that is on position \p hoverPoint
   Returns nullptr if there is no instance.
 */
InstanceItem *MessageItem::hoveredInstance(const QPointF &hoverPoint, bool extendToBottom) const
{
    const QVector<InstanceItem *> &others = instanceItemsByPos(scene(), hoverPoint, extendToBottom);
    return others.isEmpty() ? nullptr : others.first();
}

void MessageItem::commitGeometryChange()
{
    const QRectF &newBounds = mapRectFromItem(m_arrowItem, m_arrowItem->boundingRect());
    if (boundingRect() != newBounds) {
        prepareGeometryChange();
        setBoundingRect(newBounds);
    }
}

void MessageItem::onSourceInstanceMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    if (!qFuzzyIsNull(offset.x())) {
        const QPointF &desitnation = m_arrowItem->arrow()->anchorPointSource() + QPointF(offset.x(), 0);
        updateSource(desitnation, ObjectAnchor::Snap::SnapTo, m_sourceInstance);
    }

    if (!qFuzzyIsNull(offset.y()))
        updateSourceAndTarget(QPointF(0, offset.y()));

    scheduleLayoutUpdate();

    if (geometryManagedByCif())
        updateCif();
}

void MessageItem::onTargetInstanceMoved(const QPointF &from, const QPointF &to)
{
    const QPointF offset(to - from);
    if (!qFuzzyIsNull(offset.x())) {
        const QPointF &desitnation = m_arrowItem->arrow()->anchorPointTarget() + QPointF(offset.x(), 0);
        updateTarget(desitnation, ObjectAnchor::Snap::SnapTo, m_targetInstance);
    }

    if (!qFuzzyIsNull(offset.y()))
        updateSourceAndTarget(QPointF(0, offset.y()));

    scheduleLayoutUpdate();

    if (geometryManagedByCif())
        updateCif();
}

/*!
   The coordinates of the head point in scene coordinates
 */
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

/*!
   The coordinates of the tail point in scene coordinates
 */
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

void MessageItem::onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() != shared::ui::GripPoint::Location::Center || from == to || isCreator())
        return;

    const qreal shift = QPointF(to - from).y();

    QSignalBlocker suppressUpdates(this);
    const QVector<QPointF> currentPoints(messagePoints());
    if (m_originalMessagePoints.isEmpty())
        m_originalMessagePoints = currentPoints;

    const qreal newHead = head().y() + shift;
    const qreal newTail = tail().y() + shift;

    const QRectF actualContentRect = m_chartLayoutManager ? m_chartLayoutManager->actualContentRect() : QRectF();
    const qreal upperBound = qMax(m_sourceInstance ? m_sourceInstance->axis().y1() : actualContentRect.top(),
            m_targetInstance ? m_targetInstance->axis().y1() : actualContentRect.top());
    const qreal lowerBound = qMin(m_sourceInstance ? m_sourceInstance->axis().y2() : actualContentRect.bottom(),
            m_targetInstance ? m_targetInstance->axis().y2() : actualContentRect.bottom());

    if (newHead < upperBound || newHead > lowerBound || newTail < upperBound || newTail > lowerBound)
        return;

    updateSourceAndTarget(QPointF(0, shift));
}

void MessageItem::onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (from == to)
        return;

    if (isCreator())
        return;

    QSignalBlocker suppressUpdates(this);
    QVector<QPointF> currentPoints(messagePoints());
    if (m_originalMessagePoints.isEmpty())
        m_originalMessagePoints = currentPoints;

    const bool isSource = gp->location() == shared::ui::GripPoint::Left;
    auto validatePoint = [&](const QPointF &requestedPoint, int pointId) {
        QPointF point(requestedPoint);
        const QPointF &oppositePoint = currentPoints.at(pointId == 0 ? currentPoints.size() - 1 : 0);

        // Arrow direction - should be top-to-bottom:
        if ((isSource && point.y() > oppositePoint.y()) || (!isSource && point.y() < oppositePoint.y()))
            point.ry() = oppositePoint.y();

        // Anchor point - should be within an appropriate axis:
        if (InstanceItem *instance = isSource ? m_sourceInstance : m_targetInstance) {
            const QLineF &axis = instance->axis();
            if (point.y() < axis.y1())
                point.ry() = axis.y1();
            if (point.y() > axis.y2())
                point.ry() = axis.y2();
        }

        return point;
    };

    const QPointF &validated = validatePoint(to, isSource ? 0 : currentPoints.size() - 1);
    if (isSource)
        updateSource(validated, ObjectAnchor::Snap::SnapTo);
    else
        updateTarget(validated, ObjectAnchor::Snap::SnapTo);
}

void MessageItem::onManualGeometryChangeFinished(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(to);
    if (m_sourceInstance == m_targetInstance) {
        GeometryNotificationBlocker keepSilent(this);
        m_originalMessagePoints.clear();
        const bool isSource = gp->location() == shared::ui::GripPoint::Left;
        if (isSource)
            updateSource(from, ObjectAnchor::Snap::SnapTo);
        else
            updateTarget(from, ObjectAnchor::Snap::SnapTo);
        updateCif();
        commitGeometryChange();
        Q_EMIT moved(this);
        return;
    }

    onChartBoxChanged();

    bool converted(false);
    const QVector<QPoint> &oldPointsCif = CoordinatesConverter::sceneToCif(m_originalMessagePoints, &converted);
    if (m_originalMessagePoints.size() && !converted) {
        m_originalMessagePoints.clear();
        qWarning() << "MessageItem move: scene->mm original coordinates conversion failed";
        return;
    }

    const QVector<QPointF> &currPoints = messagePoints();
    const QVector<QPoint> &newPointsCif = CoordinatesConverter::sceneToCif(currPoints, &converted);
    if (!converted) {
        qWarning() << "MessageItem move: scene->mm target coordinates conversion failed";
        return;
    }

    m_originalMessagePoints.clear();

    msc::cmd::CommandsStack::current()->beginMacro(tr("Change message geometry"));

    bool sourceChanged = false;
    if (auto src = sourceInstanceItem()) {
        if (src->modelItem() != m_message->sourceInstance())
            sourceChanged = true;
    } else if (m_message->sourceInstance()) {
        sourceChanged = true;
    }

    bool targetChanged = false;
    if (auto trg = targetInstanceItem()) {
        if (trg->modelItem() != m_message->targetInstance())
            targetChanged = true;
    } else if (m_message->targetInstance()) {
        targetChanged = true;
    }

    if (sourceChanged) {
        const int newIdx = m_chartLayoutManager->eventIndex(tail().y());
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                { QVariant::fromValue<MscMessage *>(m_message), newIdx,
                        QVariant::fromValue<MscInstance *>(
                                sourceInstanceItem() ? sourceInstanceItem()->modelItem() : nullptr),
                        QVariant::fromValue<MscMessage::EndType>(MscMessage::EndType::SOURCE_TAIL) });
    }
    if (targetChanged) {
        const int newIdx = m_chartLayoutManager->eventIndex(head().y());
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                { QVariant::fromValue<MscMessage *>(m_message), newIdx,
                        QVariant::fromValue<MscInstance *>(
                                targetInstanceItem() ? targetInstanceItem()->modelItem() : nullptr),
                        QVariant::fromValue<MscMessage::EndType>(MscMessage::EndType::TARGET_HEAD) });
    }

    const qreal newPos = (gp && gp->location() == shared::ui::GripPoint::Center) ? to.y() : tail().y();
    int newIdx = m_chartLayoutManager->eventIndex(newPos, m_message);
    msc::cmd::CommandsStack::push(msc::cmd::EditMessagePoints,
            { QVariant::fromValue(m_message.data()), QVariant::fromValue(oldPointsCif),
                    QVariant::fromValue(newPointsCif), newIdx });
    cmd::CommandsStack::current()->endMacro();

    if (auto item = m_chartLayoutManager->itemForComment(m_message->comment())) {
        item->instantLayoutUpdate();
    }

    Q_EMIT moved(this);
}

MessageItem *MessageItem::createDefaultItem(
        MscMessage *message, ChartLayoutManager *chartLayoutManager, const QPointF &pos)
{
    MessageItem *messageItem = new MessageItem(message, chartLayoutManager);
    static const qreal halfLength(ArrowItem::defaultWidth() / 2.);
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
    if (isCreator()) {
        m_arrowItem->enableEditMode();
    } else {
        showMessageDialog();
    }
}

bool MessageItem::isCreator() const
{
    return m_message && m_message->messageType() == MscMessage::MessageType::Create;
}

void MessageItem::initGripPoints()
{
    InteractiveObjectBase::initGripPoints();

    if (isCreator())
        gripPointsHandler()->setUsedPoints(shared::ui::GripPoint::Locations());
    else
        gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Left,
                shared::ui::GripPoint::Location::Right, shared::ui::GripPoint::Location::Center });

    connect(gripPointsHandler(), &shared::ui::GripPointsHandler::manualGeometryChangeFinish, this,
            &MessageItem::onManualGeometryChangeFinished, Qt::UniqueConnection);

    m_arrowItem->setZValue(gripPointsHandler()->zValue() - 1);
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
    CommandsStack::push(SetParameterList,
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

/*!
   Checks in the aadl model, if there is a corresponding connection
 */
void MessageItem::checkAadlConnection()
{
    m_arrowItem->setColor(aadlConnectionOk() ? msc::MessageColor : msc::AadlErrorColor);
    updateTooltip();
}

void MessageItem::addMessagePoint(const QPointF &scenePoint)
{
    m_arrowItem->arrow()->addTurnPoint(scenePoint);
}

/*!
   Returns the message points (from source to target) in scene coordinates
 */
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

void MessageItem::setMessagePoints(const QVector<QPointF> &scenePoints)
{
    const QVector<QPointF> &arrowPoints = messagePoints();
    if (scenePoints.size() < 2 || scenePoints == arrowPoints)
        return;

    Q_ASSERT(!scenePoints.isEmpty());

    GeometryNotificationBlocker keepSilent(this);
    setPos(QLineF(scenePoints.first(), scenePoints.last()).center());
    m_arrowItem->arrow()->makeArrow(m_sourceInstance, scenePoints.first(), m_targetInstance, scenePoints.last());
    m_arrowItem->arrow()->setTurnPoints(scenePoints);

    if (geometryManagedByCif()) {
        updateCif();
    }

    updateGripPoints();
    commitGeometryChange();
}

void MessageItem::applyCif()
{
    QSignalBlocker sb(this);
    if (const cif::CifBlockShared &cifBlock = mainCifBlock()) {
        const QVector<QPoint> &pointsCif = cifBlock->payload(mainCifType()).value<QVector<QPoint>>();
        bool converted(false);
        const QVector<QPointF> &pointsScene = CoordinatesConverter::cifToScene(pointsCif, &converted);
        setMessagePoints(pointsScene);
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
        setMessagePoints(points);
}

cif::CifLine::CifType MessageItem::mainCifType() const
{
    return isCreator() ? cif::CifLine::CifType::Create : cif::CifLine::CifType::Message;
}

QVariant MessageItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged) {
        // When the message was moved, make sure the arrow start/end are moved as well
        const QVector<QPointF> &arrowPoints = messagePoints();
        if (arrowPoints.size() >= 2) {
            ObjectsLink *link = m_arrowItem->arrow()->link();
            link->source()->setPoint(arrowPoints.first(), ObjectAnchor::Snap::NoSnap);
            link->target()->setPoint(arrowPoints.last(), ObjectAnchor::Snap::NoSnap);
        }
    }

    return InteractiveObject::itemChange(change, value);
}

void MessageItem::updateCif()
{
    using namespace cif;

    const cif::CifLine::CifType usedCifType = mainCifType();
    const QVector<QPoint> &pointsCif = CoordinatesConverter::sceneToCif(messagePoints());
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

void MessageItem::extendGlobalMessage()
{
    if (!m_targetInstance && !m_sourceInstance)
        return;

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
        setMessagePoints(points);
    }
}

/*!
   Apply X position of the message target from the instance
 */
void MessageItem::applyTargetPointFromInstance()
{
    QVector<QPointF> points = messagePoints();
    if (messagePoints().size() > 1) {
        if (m_targetInstance) {
            QPointF &pt = points.last();
            if (isCreator()) {
                // Creator arrow end at the instance header border
                if (points.first().x() < pt.x()) {
                    pt.setX(m_targetInstance->leftCreatorTarget().x());
                } else {
                    pt.setX(m_targetInstance->rightCreatorTarget().x());
                }
            } else {
                const QRectF targetRect = m_targetInstance->sceneBoundingRect();
                pt.setX(targetRect.center().x());
            }
        }
        setMessagePoints(points);
    }
}

void MessageItem::onChartBoxChanged()
{
    if (!wannabeGlobal() || m_message->isOrphan())
        return;

    extendGlobalMessage();
}

QPointF MessageItem::extendToNearestEdge(const QPointF &shiftMe) const
{
    const QRectF boxRect = getChartBox();
    const QLineF left({ boxRect.left(), shiftMe.y() }, shiftMe);
    const QLineF right({ boxRect.right(), shiftMe.y() }, shiftMe);

    if (left.length() <= right.length()) {
        return QPointF(boxRect.left(), shiftMe.y());
    } else {
        return QPointF(boxRect.right(), shiftMe.y());
    }
}

/*
 * Almost the same as MscMessage::isGlobal, but the check is based on existence
 * of InstanceItems instead of MscInstances.
 *
 * During manual editing the MessageItem may became "global" without changing
 * corresponding MscInstance in the m_message which will be updated after finishing manual editing.
 */
bool MessageItem::wannabeGlobal() const
{
    const bool isRegular = m_message->messageType() == MscMessage::MessageType::Message;
    const bool onlySource = m_sourceInstance && !m_targetInstance;
    const bool onlyTarget = !m_sourceInstance && m_targetInstance;
    return isRegular && (onlySource || onlyTarget);
}

/*!
   Returns, if the message has a corresponding ssdl connection
 */
bool MessageItem::aadlConnectionOk() const
{
    if (m_chartLayoutManager && m_chartLayoutManager->aadlChecker()) {
        return m_chartLayoutManager->aadlChecker()->checkMessage(m_message);
    }
    return true;
}

} // namespace msc
