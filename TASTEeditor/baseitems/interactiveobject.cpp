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

#include "interactiveobject.h"

#include "app/commandsstack.h"
#include "baseitems/common/highlightrectitem.h"
#include "baseitems/common/utils.h"
#include "grippointshandler.h"
#include "tab_aadl/aadlobject.h"
#include "tab_interface/commands/cmdentityautolayout.h"
#include "tab_interface/commands/commandids.h"
#include "tab_interface/commands/commandsfactory.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <app/commandsstack.h>
#include <functional>

namespace taste3 {
namespace aadl {

InteractiveObject::InteractiveObject(AADLObject *entity, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_dataObject(entity)
    , m_selectedPen(Qt::black, 2, Qt::DotLine)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
             | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(ColorManager::instance(), &ColorManager::colorsUpdated, this, &InteractiveObject::applyColorScheme);
}

void InteractiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (isSelected()) {
        painter->save();
        painter->setPen(m_selectedPen);

        painter->drawRect(m_boundingRect);
        painter->restore();
    }
}

QRectF InteractiveObject::boundingRect() const
{
    return m_boundingRect;
}

AADLObject *InteractiveObject::aadlObject() const
{
    return m_dataObject;
}

void InteractiveObject::gripPointPressed(GripPoint *gp, const QPointF &at)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveStart(gp, at);
    else
        onManualResizeStart(gp, at);
}

void InteractiveObject::gripPointMoved(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveProgress(gp, from, to);
    else
        onManualResizeProgress(gp, from, to);
}

void InteractiveObject::gripPointReleased(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveFinish(gp, pressedAt, releasedAt);
    else
        onManualResizeFinish(gp, pressedAt, releasedAt);
}

void InteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(isSelected ? 1 : 0);
    if (isSelected) {
        showGripPoints();
        updateGripPoints();
    } else {
        hideGripPoints();
    }
}

void InteractiveObject::rebuildLayout() {}

QFont InteractiveObject::font() const
{
    return m_font;
}

void InteractiveObject::setFont(const QFont &font)
{
    m_font = font;
}

void InteractiveObject::updateEntity()
{
    QList<QVariant> params;
    const QList<QVariantList> preparedParams { prepareChangeCoordinatesCommandParams() };
    std::transform(preparedParams.cbegin(), preparedParams.cend(), std::back_inserter(params),
                   [](const QVariantList entryParams) { return QVariant::fromValue(entryParams); });
    const auto changeGeometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, params);
    taste3::cmd::CommandsStack::current()->push(changeGeometryCmd);
}

void InteractiveObject::mergeGeometry()
{
    QList<QVariant> params;
    const QList<QVariantList> preparedParams { prepareChangeCoordinatesCommandParams() };
    std::transform(preparedParams.cbegin(), preparedParams.cend(), std::back_inserter(params),
                   [](const QVariantList entryParams) { return QVariant::fromValue(entryParams); });

    QUndoCommand *autolayoutCmd = cmd::CommandsFactory::create(cmd::AutoLayoutEntity, params);
    autolayoutCmd->redo();

    const int cmdIdx = taste3::cmd::CommandsStack::current()->index();
    const QUndoCommand *prevCmd = taste3::cmd::CommandsStack::current()->command(cmdIdx - 1);
    if (auto prevGeometryBasedCmd = dynamic_cast<const cmd::CmdEntityGeometryChange *>(prevCmd))
        const_cast<cmd::CmdEntityGeometryChange *>(prevGeometryBasedCmd)->mergeCommand(autolayoutCmd);
    else
        delete autolayoutCmd;
}

QList<QVariantList> InteractiveObject::prepareChangeCoordinatesCommandParams() const
{
    QList<QVariantList> params;
    auto children = childItems();
    std::stable_sort(children.begin(), children.end(),
                     [](QGraphicsItem *item1, QGraphicsItem *item2) { return item1->type() < item2->type(); });

    for (auto item : children) {
        if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject()))
            params.append(iObj->prepareChangeCoordinatesCommandParams());
    }
    params.erase(std::unique(params.begin(), params.end()), params.end());
    return params;
}

QBrush InteractiveObject::brush() const
{
    return m_brush;
}

void InteractiveObject::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

QPen InteractiveObject::pen() const
{
    return m_pen;
}

void InteractiveObject::setPen(const QPen &pen)
{
    m_pen = pen;
}

void InteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(nullptr, event->lastScenePos(), event->scenePos());
}

void InteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(nullptr, event->scenePos());
}

void InteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    onManualMoveFinish(nullptr, event->buttonDownScenePos(event->button()), event->scenePos());
    emit clicked();
    QGraphicsObject::mouseReleaseEvent(event);
}

void InteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void InteractiveObject::onManualMoveStart(GripPoint *gp, const QPointF &at)
{
    Q_UNUSED(gp)
    Q_UNUSED(at)
}

void InteractiveObject::onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp)
    Q_UNUSED(from)
    Q_UNUSED(to)
}

void InteractiveObject::onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(gp)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)
}

void InteractiveObject::onManualGripPointAdd(GripPoint *gp)
{
    Q_UNUSED(gp)
}

void InteractiveObject::onManualGripPointRemove(GripPoint *gp)
{
    Q_UNUSED(gp)
}

void InteractiveObject::onManualResizeStart(GripPoint *gp, const QPointF &at)
{
    Q_UNUSED(gp)
    Q_UNUSED(at)
}

void InteractiveObject::onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(gp)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)
}

void InteractiveObject::onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp)
    Q_UNUSED(from)
    Q_UNUSED(to)
}

void InteractiveObject::hideGripPoints()
{
    if (m_gripPointsHandler)
        m_gripPointsHandler->hideAnimated();
}

void InteractiveObject::showGripPoints()
{
    initGripPoints();
    m_gripPointsHandler->showAnimated();
}

void InteractiveObject::initGripPoints()
{
    if (m_gripPointsHandler)
        return;

    m_gripPointsHandler = new GripPointsHandler(this);
    m_gripPointsHandler->setZValue(utils::kGripZLevel);

    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeStart, this,
            &InteractiveObject::gripPointPressed);
    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeProgress, this,
            &InteractiveObject::gripPointMoved);
    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeFinish, this,
            &InteractiveObject::gripPointReleased);

    connect(m_gripPointsHandler, &GripPointsHandler::visibleChanged, this, [this]() {
        if (m_gripPointsHandler && !m_gripPointsHandler->isVisible())
            delete m_gripPointsHandler; // it's not a thing directly added to the scene, so just delete is enough
    });
}

void InteractiveObject::updateGripPoints()
{
    if (m_gripPointsHandler)
        m_gripPointsHandler->updateLayout();
}

QVariant InteractiveObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemSelectedChange:
        onSelectionChanged(value.toBool());
        break;
    case QGraphicsItem::ItemPositionChange:
        m_prevPos = pos();
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        Q_EMIT relocated(m_prevPos, pos());
        break;
    default:
        break;
    }

    return QGraphicsObject::itemChange(change, value);
}

bool InteractiveObject::isHovered() const
{
    if (!isUnderMouse())
        return false;

    return m_gripPointsHandler && m_gripPointsHandler->isVisible();
}

HighlightRectItem *InteractiveObject::createHighlighter()
{
    if (m_highlighter)
        delete m_highlighter;
    m_highlighter = new HighlightRectItem(this);
    connect(m_highlighter, &HighlightRectItem::highlighted, m_highlighter, &QObject::deleteLater);
    connect(m_highlighter, &QObject::destroyed, this, [this]() { m_highlighter = nullptr; });

    m_highlighter->setPath(shape());

    return m_highlighter;
}

bool InteractiveObject::isHighlightable() const
{
    return m_highlightable;
}

void InteractiveObject::setHighlightable(bool highlightable)
{
    m_highlightable = highlightable;
    clearHighlight();
}

void InteractiveObject::highlightConnected()
{
    doHighlighting(Qt::green, false);
}

void InteractiveObject::highlightDisconnected()
{
    doHighlighting(Qt::red, false);
}

void InteractiveObject::doHighlighting(const QColor &color, bool permanent)
{
    if (!m_highlightable) {
        return;
    }

    if (HighlightRectItem *highlighter = createHighlighter()) {
        QColor targetColor(color);
        QPen p(targetColor);
        p.setWidthF(1.);
        highlighter->setPen(p);
        targetColor.setAlphaF(0.25);
        highlighter->setBrush(targetColor);

        if (!permanent)
            highlighter->highlight();
    }
}

void InteractiveObject::clearHighlight()
{
    if (m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
    }
}

bool InteractiveObject::isHighlighting() const
{
    return m_highlighter != nullptr;
}

QPointF InteractiveObject::centerInScene() const
{
    return sceneBoundingRect().center();
}

void InteractiveObject::scheduleLayoutUpdate()
{
    if (m_layoutDirty)
        return;

    m_layoutDirty = QMetaObject::invokeMethod(this, "instantLayoutUpdate", Qt::QueuedConnection);
}

void InteractiveObject::instantLayoutUpdate()
{
    const QRectF oldBounds = sceneBoundingRect();

    rebuildLayout();
    m_layoutDirty = false;

    if (oldBounds != sceneBoundingRect())
        Q_EMIT boundingBoxChanged();

    update();
}

aadl::ColorHandler InteractiveObject::colorHandler() const
{
    ColorHandler h = ColorManager::colorsForItem(handledColorType());
    if (AADLObject *aadlObj = aadlObject()) {
        if (aadlObj->props().contains("color")) { // keep single custom color
            h.setFillType(ColorHandler::Color);
            h.setBrushColor0(QColor(aadlObj->props().value("color").toString()));
        }
    }

    return h;
}

QString InteractiveObject::prepareTooltip() const
{
    return aadlObject() ? aadlObject()->title() : QString();
}

} // namespace aadl
} // namespace taste3
