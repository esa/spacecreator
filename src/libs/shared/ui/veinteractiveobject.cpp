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
#include "textitem.h"
#include "veobject.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

namespace shared {
namespace ui {

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

void VEInteractiveObject::mergeGeometry()
{
    QTimer::singleShot(0, this, [this]() {
        if (!m_commandsStack) {
            qWarning() << Q_FUNC_INFO << "No command stack set in shared::ui::VEInteractiveObject";
            return;
        }

        for (auto child : childItems()) {
            if (auto io = qobject_cast<VEInteractiveObject *>(child->toGraphicsObject())) {
                io->mergeGeometry();
            }
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
    });
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

void VEInteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(gripPointItem(shared::ui::GripPoint::Center), event->lastScenePos(), event->scenePos());
}

void VEInteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(gripPointItem(shared::ui::GripPoint::Center), event->scenePos());
}

void VEInteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    onManualMoveFinish(gripPointItem(shared::ui::GripPoint::Center), event->buttonDownScenePos(event->button()),
            event->scenePos());
    Q_EMIT clicked();
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
    m_textItem->setHtml(entity()->titleUI());
    updateTextPosition();
}

}

}
