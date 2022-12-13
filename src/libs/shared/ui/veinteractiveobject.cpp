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
static const qreal extraSpace = 5.0;

VEInteractiveObject::VEInteractiveObject(VEObject *entity, QGraphicsItem *parent)
    : ui::InteractiveObjectBase(parent)
    , m_dataObject(entity)
    , m_textItem(nullptr)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemSendsScenePositionChanges |
             QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);

    connect(shared::ColorManager::instance(), &shared::ColorManager::colorsUpdated,
            this, &VEInteractiveObject::applyColorScheme);

    connect(this, &VEInteractiveObject::boundingBoxChanged,
            this, &VEInteractiveObject::updateTextPosition);
}

void VEInteractiveObject::init()
{
    m_textItem = initTextItem();

    applyColorScheme();
}

TextItem *VEInteractiveObject::initTextItem()
{
    auto textItem = new TextItem(this);
    textItem->setEditable(true);
    textItem->setFont(font());
    textItem->setBackground(Qt::transparent);
    textItem->setTextWrapMode(QTextOption::NoWrap);
    textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
    textItem->setOpenExternalLinks(true);
    return textItem;
}

shared::VEObject *VEInteractiveObject::entity() const
{
    return m_dataObject;
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

void VEInteractiveObject::updateText()
{
    if (!m_textItem)
    {
        return;
    }

    const QString text = entity()->titleUI();
    if (Qt::mightBeRichText(text))
    {
        if (text != m_textItem->toHtml()) {
            m_textItem->setHtml(text);
        }
    }
    else if (text != m_textItem->toPlainText())
    {
        m_textItem->setPlainText(text);
    }

    // This class cannot know what kind of text is being rendered, so
    // positioning is left to the children through a pure virtual method
    updateTextPosition();
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

void VEInteractiveObject::rebuildLayout()
{
    updateGripPoints();
    applyColorScheme();
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
    // Get colorHandler for the type of this instance type
    shared::ColorHandler h = shared::ColorManager::instance()->colorsForItem(handledColorType());
    // Read color from entity (the model) and set it on the colorhandler
    if (auto ivObj = entity())
    {
        if (ivObj->hasEntityAttribute(QLatin1String("color"))) { // keep single custom color
            h.setFillType(shared::ColorHandler::Color);
            h.setBrushColor0(QColor(ivObj->entityAttributeValue<QString>(QLatin1String("color"))));
        }
    }

    return h;
}

void VEInteractiveObject::setCommandsStack(cmd::CommandsStackBase *commandsStack)
{
    m_commandsStack = commandsStack;
}

QString VEInteractiveObject::prepareTooltip() const
{
    return entity() ? entity()->titleUI() : QString();
}

QFont VEInteractiveObject::font() const
{
    return m_font;
}

void VEInteractiveObject::setFont(const QFont &font)
{
    m_font = font;
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

QString VEInteractiveObject::toString() const
{
    QString typeName = QString("VEInteractiveObject: ");
    if (m_textItem == nullptr)
    {
        return typeName;
    }

    QString name;
    if (m_textItem->textIsValid())
    {
        name = m_textItem->toPlainText();
    }
    else
    {
        name = "NoName";
    }

    auto br = sceneBoundingRect();
    auto x = br.x();
    auto y = br.y();
    auto w = br.width();
    auto h = br.height();
    auto result = QString(typeName + name + " rect: %1,%2, (%3,%4)").arg(x).arg(y).arg(w).arg(h);
    return result;
}

QDebug operator<<(QDebug debug, const shared::ui::VEInteractiveObject &veobj)
{
    debug << veobj.toString();
    return debug;
}
} // namespace ui
} // namespace shared
