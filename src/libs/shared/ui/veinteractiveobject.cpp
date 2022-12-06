/*
  Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "veinteractiveobject.h"

#include "commands/cmdentityautolayout.h"
#include "commands/cmdentitygeometrychange.h"
#include "commandsstackbase.h"
#include "graphicsviewutils.h"
#include "textitem.h"
#include "veobject.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTimer>

namespace shared {
namespace ui {

static const qreal kClickTreshold = 5;

VEInteractiveObject::VEInteractiveObject(VEObject *entity, QGraphicsItem *parent)
    : ui::InteractiveObjectBase(parent)
    , m_dataObject(entity)
    , m_textItem(nullptr)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
            | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(shared::ColorManager::instance(), &shared::ColorManager::colorsUpdated, this,
            &VEInteractiveObject::applyColorScheme);

    connect(this, &VEInteractiveObject::boundingBoxChanged, this, &VEInteractiveObject::updateTextPosition);
}

shared::VEObject *VEInteractiveObject::entity() const
{
    return m_dataObject;
}

void VEInteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(itemLevel(isSelected));
    if (isSelected) {
        showGripPoints();
        updateGripPoints();
    } else {
        hideGripPoints();
    }
}

void VEInteractiveObject::childBoundingBoxChanged()
{
    scheduleLayoutUpdate();
}

void VEInteractiveObject::mergeGeometry()
{
#ifdef __NONE__
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in shared::ui::VEInteractiveObject";
        return;
    }

    const QList<QPair<shared::VEObject *, QVector<QPointF>>> geometryData = prepareChangeCoordinatesCommandParams();
    const QUndoCommand *cmd = m_commandsStack->command(m_commandsStack->index() - 1);
    if (auto prevGeometryBasedCmd = dynamic_cast<const cmd::CmdEntityGeometryChange *>(cmd)) {
        auto mutCmd = const_cast<cmd::CmdEntityGeometryChange *>(prevGeometryBasedCmd);
        if (mutCmd->mergeGeometryData(geometryData)) {
            return;
        }
    }
    m_commandsStack->push(new cmd::CmdEntityAutoLayout(geometryData));
#endif
}

QFont VEInteractiveObject::font() const
{
    return m_font;
}

void VEInteractiveObject::setFont(const QFont &font)
{
    m_font = font;
}

void VEInteractiveObject::init()
{
    m_textItem = initTextItem();
    applyColorScheme();
}

void VEInteractiveObject::updateEntity()
{
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in shared::ui::VEInteractiveObject";
        return;
    }

    const auto changeGeometryCmd = new cmd::CmdEntityGeometryChange(prepareChangeCoordinatesCommandParams());
    m_commandsStack->push(changeGeometryCmd);
}

void VEInteractiveObject::rebuildLayout()
{
    updateGripPoints();
    applyColorScheme();
}

QList<QPair<shared::VEObject *, QVector<QPointF>>> VEInteractiveObject::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<shared::VEObject *, QVector<QPointF>>> params;
    auto children = childItems();
    std::stable_sort(children.begin(), children.end(),
            [](QGraphicsItem *item1, QGraphicsItem *item2) { return item1->type() < item2->type(); });

    for (auto item : qAsConst(children)) {
        if (auto iObj = qobject_cast<VEInteractiveObject *>(item->toGraphicsObject()))
            params.append(iObj->prepareChangeCoordinatesCommandParams());
    }
    params.erase(std::unique(params.begin(), params.end()), params.end());
    return params;
}

QBrush VEInteractiveObject::brush() const
{
    return m_brush;
}

void VEInteractiveObject::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

QPen VEInteractiveObject::pen() const
{
    return m_pen;
}

void VEInteractiveObject::setPen(const QPen &pen)
{
    m_pen = pen;
}

void VEInteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    s_mouseReleased = false;
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(gripPointItem(shared::ui::GripPoint::Center), event->scenePos());
}

void VEInteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // We keep track of whether the mouse has been released already or not. If
    // the release happens at an edge, and it causes resizing the canvas,
    // sometimes it makes the scrollbar of the canvas resize and move, which
    // makes the graphics scene send an additional mouse move even that we want
    // to discard, or the element ends out of the screen.
    if (s_mouseReleased) {
        return;
    }
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(gripPointItem(shared::ui::GripPoint::Center), event->lastScenePos(), event->scenePos());
}

void VEInteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    s_mouseReleased = true;
    onManualMoveFinish(gripPointItem(shared::ui::GripPoint::Center), event->buttonDownScenePos(event->button()),
            event->scenePos());

    const qreal distance = graphicsviewutils::distanceLine(event->buttonDownScenePos(event->button()), event->scenePos());
    if (distance <= kClickTreshold)
        Q_EMIT clicked(event->scenePos());
    QGraphicsObject::mouseReleaseEvent(event);
}

void VEInteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_EMIT doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

shared::ColorHandler VEInteractiveObject::colorHandler() const
{
    shared::ColorHandler h = shared::ColorManager::instance()->colorsForItem(handledColorType());
    if (auto ivObj = entity()) {
        if (ivObj->hasEntityAttribute(QLatin1String("color"))) { // keep single custom color
            h.setFillType(shared::ColorHandler::Color);
            h.setBrushColor0(QColor(ivObj->entityAttributeValue<QString>(QLatin1String("color"))));
        }
    }

    return h;
}

TextItem *VEInteractiveObject::initTextItem()
{
    auto textItem = new TextItem(this);
    textItem->setEditable(true);
    textItem->setFont(font());
    textItem->setBackground(Qt::transparent);
    textItem->setTextWrapMode(QTextOption::WrapAnywhere);
    textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textItem->setOpenExternalLinks(true);
    return textItem;
}

void VEInteractiveObject::updateTextPosition()
{
    if (m_textItem) {
        m_textItem->setExplicitSize(boundingRect().size());
    }
}

void VEInteractiveObject::setCommandsStack(cmd::CommandsStackBase *commandsStack)
{
    m_commandsStack = commandsStack;
}

QString VEInteractiveObject::prepareTooltip() const
{
    return entity() ? entity()->titleUI() : QString();
}

void VEInteractiveObject::updateText()
{
    if (!m_textItem) {
        return;
    }

    const QString text = entity()->titleUI();
    if (Qt::mightBeRichText(text)) {
        if (text != m_textItem->toHtml()) {
            m_textItem->setHtml(text);
        }
    } else if (text != m_textItem->toPlainText()) {
        m_textItem->setPlainText(entity()->titleUI());
    }
    updateTextPosition();
}

} // namespace ui
} // namespace shared
