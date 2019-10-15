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

#include "aadlfunctiongraphicsitem.h"

#include "aadlconnectiongraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "tab_aadl/aadlcommonprops.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectconnection.h"
#include "tab_aadl/aadlobjectfunction.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QtDebug>
#include <app/commandsstack.h>
#include <baseitems/textgraphicsitem.h>
#include <baseitems/grippointshandler.h>

static const qreal kBorderWidth = 2;

namespace taste3 {
namespace aadl {

AADLFunctionGraphicsItem::AADLFunctionGraphicsItem(AADLObjectFunction *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_textItem(new TextGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLFunctionGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    updateColors();

    m_textItem->setEditable(true);
    m_textItem->setPlainText(entity->title());
    m_textItem->setFont(QFont(qApp->font().family(), 16, QFont::Bold, true));
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemIsSelectable);

    connect(m_textItem, &TextGraphicsItem::edited, this, [this](const QString &text) {
        const QVariantMap attributess = { { meta::token(meta::Token::name), text } };
        const auto attributesCmd = cmd::CommandsFactory::create(
                cmd::ChangeEntityAttributes, { qVariantFromValue(modelEntity()), qVariantFromValue(attributess) });
        taste3::cmd::CommandsStack::current()->push(attributesCmd);
    });
    connect(entity, &AADLObjectFunction::attributesChanged, this, [this, entity]() {
        if (m_textItem->toPlainText() != entity->title())
            m_textItem->setPlainText(entity->title());
        instantLayoutUpdate();
    });
    connect(entity, &AADLObjectFunction::titleChanged, this, [this](const QString &text) {
        m_textItem->setPlainText(text);
        instantLayoutUpdate();
    });
}

AADLObjectFunction *AADLFunctionGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunction *>(m_entity);
}

QList<QVariantList> AADLFunctionGraphicsItem::prepareConnectionsParams() const
{
    QList<QVariantList> params;
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            if (AADLConnectionGraphicsItem *connection = iface->connectedItem())
                params.append({ qVariantFromValue(connection->entity()), qVariantFromValue(connection->points()) });
            params.append(
                    { qVariantFromValue(iface->entity()), qVariantFromValue<QVector<QPointF>>({ iface->scenePos() }) });
        }
    }
    return params;
}

void AADLFunctionGraphicsItem::createCommand()
{
    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change item geometry/position"));

    for (auto connectionParams : prepareConnectionsParams()) {
        const auto linkedItemsGeometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, connectionParams);
        taste3::cmd::CommandsStack::current()->push(linkedItemsGeometryCmd);
    }

    const QRectF geometry = sceneBoundingRect();
    const QVector<QPointF> points { geometry.topLeft(), geometry.bottomRight() };
    const auto geometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry,
                                                          { qVariantFromValue(entity()), qVariantFromValue(points) });
    taste3::cmd::CommandsStack::current()->push(geometryCmd);

    taste3::cmd::CommandsStack::current()->endMacro();
}

QSizeF AADLFunctionGraphicsItem::defaultSize()
{
    return { 200, 80 };
}

void AADLFunctionGraphicsItem::rebuildLayout()
{
    if (auto graphicsItemParent = parentItem()) {
        QPointF newPos = pos();
        if (parentItem()) {
            const QRectF contentRect = graphicsItemParent->boundingRect();
            if (newPos.x() < contentRect.left())
                newPos.setX(contentRect.left());
            else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
                newPos.setX(contentRect.right() - m_boundingRect.width());
            if (newPos.y() < contentRect.top())
                newPos.setY(contentRect.top());
            else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
                newPos.setY(contentRect.bottom() - m_boundingRect.height());
            setPos(newPos);
        }
    }
    updateConnections();
    InteractiveObject::rebuildLayout();
    updateTextPosition();
}

void AADLFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRoundedRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2), 10, 10);
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

QVariant AADLFunctionGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemParentHasChanged)
        updateColors();
    else if (change == QGraphicsItem::ItemTransformHasChanged)
        updateTextPosition();
    return InteractiveObject::itemChange(change, value);
}

void AADLFunctionGraphicsItem::onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                  const QPointF &releasedAt)
{
    Q_UNUSED(grip);

    if (handlePositionChanged(pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualMoveProgress(grip, from, to);
    updateConnections();
}

void AADLFunctionGraphicsItem::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                    const QPointF &releasedAt)
{
    if (handleGeometryChanged(grip, pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualResizeProgress(grip, from, to);
    updateConnections();
}

void AADLFunctionGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Top, GripPoint::Location::Left, GripPoint::Location::Bottom,
                                  GripPoint::Location::Right, GripPoint::Location::TopLeft,
                                  GripPoint::Location::BottomLeft, GripPoint::Location::TopRight,
                                  GripPoint::Location::BottomRight });
}

QSizeF AADLFunctionGraphicsItem::minimalSize() const
{
    return { qMax(m_textItem->boundingRect().width(), defaultSize().width()),
             qMax(m_textItem->boundingRect().height(), defaultSize().height()) };
}

void AADLFunctionGraphicsItem::updateColors()
{
    QColor brushColor { QLatin1String("#ffd11a") };
    if (auto parentContainer = qgraphicsitem_cast<AADLFunctionGraphicsItem *>(parentItem()))
        brushColor = parentContainer->brush().color().darker(125);
    setBrush(brushColor);
    setPen(QPen(brushColor.darker(), 2));
}

void AADLFunctionGraphicsItem::updateTextPosition()
{
    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    QRectF textRect { 0, 0, m_textItem->boundingRect().width() / currScale.x(),
                      m_textItem->boundingRect().height() / currScale.y() };
    textRect.moveCenter(boundingRect().center());
    m_textItem->setPos(textRect.topLeft());
}

void AADLFunctionGraphicsItem::updateConnections()
{
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            if (AADLConnectionGraphicsItem *connection = iface->connectedItem())
                connection->instantLayoutUpdate();
        }
    }
}

} // namespace aadl
} // namespace taste3
