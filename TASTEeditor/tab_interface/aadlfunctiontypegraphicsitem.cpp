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

#include "aadlfunctiontypegraphicsitem.h"

#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
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
#include <baseitems/grippointshandler.h>
#include <baseitems/textgraphicsitem.h>

static const qreal kBorderWidth = 2;
static const QMarginsF kTextMargins = { 20, 20, 20, 20 };

namespace taste3 {
namespace aadl {

AADLFunctionTypeGraphicsItem::AADLFunctionTypeGraphicsItem(AADLObjectFunctionType *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_textItem(new AADLFunctionNameGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLFunctionTypeGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);
    setBrush(QBrush(QLatin1String("#a8a8a8")));

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

AADLObjectFunctionType *AADLFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunctionType *>(modelEntity());
}

QList<QVariantList> AADLFunctionTypeGraphicsItem::prepareAdditionalParams() const
{
    QList<QVariantList> params;
    for (auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            if (AADLConnectionGraphicsItem *connection = iface->connectedItem())
                params.append({ qVariantFromValue(connection->entity()), qVariantFromValue(connection->points()) });
            params.append(
                    { qVariantFromValue(iface->entity()), qVariantFromValue<QVector<QPointF>>({ iface->scenePos() }) });
        } /// CHECK: nested Funtion(Type) Items?
    }
    return params;
}

void AADLFunctionTypeGraphicsItem::createCommand()
{
    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change item geometry/position"));

    for (auto connectionParams : prepareAdditionalParams()) {
        const auto linkedItemsGeometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, connectionParams);
        taste3::cmd::CommandsStack::current()->push(linkedItemsGeometryCmd);
    }

    const QRectF geometry = sceneBoundingRect();
    const QVector<QPointF> points { geometry.topLeft(), geometry.bottomRight() };
    const auto geometryCmd = cmd::CommandsFactory::create(
            cmd::ChangeEntityGeometry,
            { qVariantFromValue(qobject_cast<AADLObject *>(modelEntity())), qVariantFromValue(points) });
    taste3::cmd::CommandsStack::current()->push(geometryCmd);

    taste3::cmd::CommandsStack::current()->endMacro();
}

void AADLFunctionTypeGraphicsItem::rebuildLayout()
{
    if (auto graphicsItemParent = parentItem()) {
        QPointF newPos = pos();
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
    InteractiveObject::rebuildLayout();
    updateTextPosition();
}

void AADLFunctionTypeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? m_selectedPen : m_pen);
    painter->setBrush(brush());
    painter->drawRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2));
    painter->restore();
}

QVariant AADLFunctionTypeGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemTransformHasChanged)
        updateTextPosition();
    return InteractiveObject::itemChange(change, value);
}

void AADLFunctionTypeGraphicsItem::onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                      const QPointF &releasedAt)
{
    Q_UNUSED(grip);

    if (handlePositionChanged(pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionTypeGraphicsItem::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                        const QPointF &releasedAt)
{
    if (handleGeometryChanged(grip, pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionTypeGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Top, GripPoint::Location::Left, GripPoint::Location::Bottom,
                                  GripPoint::Location::Right, GripPoint::Location::TopLeft,
                                  GripPoint::Location::BottomLeft, GripPoint::Location::TopRight,
                                  GripPoint::Location::BottomRight });
}

QSizeF AADLFunctionTypeGraphicsItem::minimalSize() const
{
    QRectF br;
    for (QGraphicsItem *item : childItems()) {
        if (item->type() == AADLFunctionTypeGraphicsItem::Type)
            br |= item->boundingRect();
        else if (item->type() == QGraphicsTextItem::Type)
            br |= item->boundingRect().marginsAdded(kTextMargins);
    }
    return br.size();
}

void AADLFunctionTypeGraphicsItem::updateTextPosition()
{
    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    QRectF textRect { 0, 0, m_textItem->boundingRect().width() / currScale.x(),
                      m_textItem->boundingRect().height() / currScale.y() };
    textRect.moveTopLeft(boundingRect().marginsRemoved(kTextMargins).topLeft());
    m_textItem->setPos(textRect.topLeft());
}

} // namespace aadl
} // namespace taste3
