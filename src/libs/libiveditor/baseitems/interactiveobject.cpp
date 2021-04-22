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

#include "baseitems/common/ivutils.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityautolayout.h"
#include "interface/commands/cmdentitygeometrychange.h"
#include "ivobject.h"

#include <QBrush>
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <functional>

namespace ive {

InteractiveObject::InteractiveObject(ivm::IVObject *entity, QGraphicsItem *parent)
    : shared::ui::InteractiveObjectBase(parent)
    , m_dataObject(entity)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
            | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(shared::ColorManager::instance(), &shared::ColorManager::colorsUpdated, this,
            &InteractiveObject::applyColorScheme);
}

ivm::IVObject *InteractiveObject::entity() const
{
    return m_dataObject;
}

void InteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(itemLevel(entity(), isSelected));
    if (isSelected) {
        showGripPoints();
        updateGripPoints();
    } else {
        hideGripPoints();
    }
}

QFont InteractiveObject::font() const
{
    return m_font;
}

void InteractiveObject::setFont(const QFont &font)
{
    m_font = font;
}

void InteractiveObject::init()
{
    applyColorScheme();
}

void InteractiveObject::updateEntity()
{
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in InteractiveObject";
        return;
    }

    const auto changeGeometryCmd = new cmd::CmdEntityGeometryChange(prepareChangeCoordinatesCommandParams());

    m_commandsStack->push(changeGeometryCmd);
}

void InteractiveObject::mergeGeometry()
{
    QTimer::singleShot(0, this, [this]() {
        if (!m_commandsStack) {
            qWarning() << Q_FUNC_INFO << "No command stack set in InteractiveObject";
            return;
        }

        for (auto child : childItems()) {
            if (auto io = qobject_cast<InteractiveObject *>(child->toGraphicsObject())) {
                io->mergeGeometry();
            }
        }

        QUndoCommand *autolayoutCmd = new cmd::CmdEntityAutoLayout(prepareChangeCoordinatesCommandParams());
        autolayoutCmd->redo();

        const QUndoCommand *cmd = m_commandsStack->command(m_commandsStack->index() - 1);
        if (auto prevGeometryBasedCmd = dynamic_cast<const cmd::CmdEntityGeometryChange *>(cmd))
            const_cast<cmd::CmdEntityGeometryChange *>(prevGeometryBasedCmd)->mergeCommand(autolayoutCmd);
        else
            delete autolayoutCmd;
    });
}

QList<QPair<ivm::IVObject *, QVector<QPointF>>> InteractiveObject::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<ivm::IVObject *, QVector<QPointF>>> params;
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

void InteractiveObject::updateGraphicsItem()
{
    updateFromEntity();
    update();
}

void InteractiveObject::rebuildLayout()
{
    setVisible(m_dataObject
            && (nestingLevel(m_dataObject) >= ive::kNestingVisibilityLevel || m_dataObject->isRootObject())
            && m_dataObject->isVisible());
}

void InteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(gripPointItem(shared::ui::GripPoint::Center), event->lastScenePos(), event->scenePos());
}

void InteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(gripPointItem(shared::ui::GripPoint::Center), event->scenePos());
}

void InteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    onManualMoveFinish(gripPointItem(shared::ui::GripPoint::Center), event->buttonDownScenePos(event->button()),
            event->scenePos());
    Q_EMIT clicked();
    QGraphicsObject::mouseReleaseEvent(event);
}

void InteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_EMIT doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

shared::ColorHandler InteractiveObject::colorHandler() const
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

QString InteractiveObject::prepareTooltip() const
{
    return entity() ? entity()->titleUI() : QString();
}

void InteractiveObject::setCommandsStack(cmd::CommandsStack *commandsStack)
{
    m_commandsStack = commandsStack;
}

}
