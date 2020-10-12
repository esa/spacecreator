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

#include "aadlobject.h"
#include "baseitems/common/aadlutils.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityautolayout.h"
#include "interface/commands/commandids.h"
#include "interface/commands/commandsfactory.h"

#include <QBrush>
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <functional>

namespace aadlinterface {

InteractiveObject::InteractiveObject(aadl::AADLObject *entity, QGraphicsItem *parent)
    : shared::ui::InteractiveObjectBase(parent)
    , m_dataObject(entity)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
            | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(ColorManager::instance(), &ColorManager::colorsUpdated, this, &InteractiveObject::applyColorScheme);
}

aadl::AADLObject *InteractiveObject::aadlObject() const
{
    return m_dataObject;
}

void InteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(itemLevel(aadlObject(), isSelected));
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
    QList<QVariant> params;
    const QList<QVariantList> preparedParams { prepareChangeCoordinatesCommandParams() };
    std::transform(preparedParams.cbegin(), preparedParams.cend(), std::back_inserter(params),
            [](const QVariantList entryParams) { return QVariant::fromValue(entryParams); });
    const auto changeGeometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, params);
    cmd::CommandsStack::current()->push(changeGeometryCmd);
}

void InteractiveObject::mergeGeometry()
{
    for (auto child : childItems()) {
        if (auto io = qobject_cast<InteractiveObject *>(child->toGraphicsObject())) {
            io->mergeGeometry();
        }
    }

    QList<QVariant> params;
    const QList<QVariantList> preparedParams { prepareChangeCoordinatesCommandParams() };
    std::transform(preparedParams.cbegin(), preparedParams.cend(), std::back_inserter(params),
            [](const QVariantList entryParams) { return QVariant::fromValue(entryParams); });

    QUndoCommand *autolayoutCmd = cmd::CommandsFactory::create(cmd::AutoLayoutEntity, params);
    autolayoutCmd->redo();

    const int cmdIdx = cmd::CommandsStack::current()->index();
    const QUndoCommand *prevCmd = cmd::CommandsStack::current()->command(cmdIdx - 1);
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

void InteractiveObject::updateGraphicsItem()
{
    updateFromEntity();
    update();
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
    Q_EMIT clicked();
    QGraphicsObject::mouseReleaseEvent(event);
}

void InteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_EMIT doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

ColorHandler InteractiveObject::colorHandler() const
{
    ColorHandler h = ColorManager::colorsForItem(handledColorType());
    if (auto aadlObj = aadlObject()) {
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

}
