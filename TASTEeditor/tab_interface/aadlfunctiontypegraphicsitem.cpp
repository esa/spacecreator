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
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "colors/colormanager.h"
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
#include <baseitems/common/utils.h>
#include <baseitems/grippointshandler.h>
#include <baseitems/textgraphicsitem.h>

static const qreal kBorderWidth = 2;

namespace taste3 {
namespace aadl {

AADLFunctionTypeGraphicsItem::AADLFunctionTypeGraphicsItem(AADLObjectFunctionType *entity, QGraphicsItem *parent)
    : InteractiveObject(entity, parent)
    , m_textItem(new AADLFunctionNameGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLFunctionTypeGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    if (entity) {
        m_textItem->setPlainText(entity->title());
        m_textItem->setFont(QFont(qApp->font().family(), 16, QFont::Bold, true));
        m_textItem->setBackgroundColor(Qt::transparent);
        m_textItem->setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemIsSelectable);

        connect(m_textItem, &TextGraphicsItem::edited, this, [this](const QString &text) {
            const QVariantMap attributess = { { meta::Props::token(meta::Props::Token::name), text } };
            const auto attributesCmd = cmd::CommandsFactory::create(
                    cmd::ChangeEntityAttributes, { QVariant::fromValue(modelEntity()), QVariant::fromValue(attributess) });
            taste3::cmd::CommandsStack::current()->push(attributesCmd);
        });
        connect(entity, &AADLObjectFunction::attributeChanged, this,
                [this, entity](taste3::aadl::meta::Props::Token attr) {
                    if (attr == taste3::aadl::meta::Props::Token::name) {
                        if (m_textItem->toPlainText() != entity->title())
                            m_textItem->setPlainText(entity->title());
                        instantLayoutUpdate();
                    }
                });
        connect(entity, &AADLObjectFunction::titleChanged, this, [this](const QString &text) {
            m_textItem->setPlainText(text);
            instantLayoutUpdate();
        });
    }
    colorSchemeUpdated();
}

AADLObjectFunctionType *AADLFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectFunctionType *>(modelEntity());
}

void AADLFunctionTypeGraphicsItem::updateFromEntity()
{
    aadl::AADLObjectFunctionType *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QRectF itemSceneRect { utils::rect(obj->coordinates()) };
    if (!itemSceneRect.isValid())
        instantLayoutUpdate();
    else
        setRect(itemSceneRect);
}

QList<QVariantList> AADLFunctionTypeGraphicsItem::prepareCommandParams() const
{
    QList<QVariantList> params;
    AADLObject *obj = qobject_cast<AADLObject *>(modelEntity());
    if (!obj->isRootObject()) {
        const QRectF geometry = sceneBoundingRect();
        const QVector<QPointF> points { geometry.topLeft(), geometry.bottomRight() };
        params.append({ QVariant::fromValue(obj), QVariant::fromValue(points) });
    }

    QList<QGraphicsItem *> items;
    static const QList<int> acceptableItemTypes { AADLConnectionGraphicsItem::Type, AADLFunctionGraphicsItem::Type,
                                                  AADLFunctionTypeGraphicsItem::Type, AADLInterfaceGraphicsItem::Type };
    for (auto item : childItems()) {
        if (acceptableItemTypes.contains(item->type()))
            items.append(item);
    }
    std::sort(items.begin(), items.end(),
              [](QGraphicsItem *item1, QGraphicsItem *item2) { return item1->type() < item2->type(); });

    for (auto item : items) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(item)) {
            params.append(
                    { QVariant::fromValue(iface->entity()), QVariant::fromValue<QVector<QPointF>>({ iface->scenePos() }) });
            for (auto outerConnection : iface->connectionItems()) {
                if (outerConnection && outerConnection->parentItem() != this) {
                    params.append({ QVariant::fromValue(outerConnection->entity()),
                                    QVariant::fromValue(outerConnection->graphicsPoints()) });
                }
            }
        } else if (auto connection = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item)) {
            params.append({ QVariant::fromValue(connection->entity()), QVariant::fromValue(connection->graphicsPoints()) });
        } else if (auto functionType = qgraphicsitem_cast<aadl::AADLFunctionTypeGraphicsItem *>(item)) {
            params.append(functionType->prepareCommandParams());
        } else if (auto function = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(item)) {
            params.append(function->prepareCommandParams());
        }
    }
    return params;
}

void AADLFunctionTypeGraphicsItem::createCommand()
{
    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change item geometry/position"));

    for (auto commandParam : prepareCommandParams()) {
        const auto changeGeometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, commandParam);
        taste3::cmd::CommandsStack::current()->push(changeGeometryCmd);
    }

    taste3::cmd::CommandsStack::current()->endMacro();
}

void AADLFunctionTypeGraphicsItem::rebuildLayout()
{
    //    if (auto graphicsItemParent = parentItem()) {
    //        QPointF newPos = pos();
    //        const QRectF contentRect = graphicsItemParent->boundingRect();
    //        if (newPos.x() < contentRect.left())
    //            newPos.setX(contentRect.left());
    //        else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
    //            newPos.setX(contentRect.right() - m_boundingRect.width());
    //        if (newPos.y() < contentRect.top())
    //            newPos.setY(contentRect.top());
    //        else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
    //            newPos.setY(contentRect.bottom() - m_boundingRect.height());
    //        setPos(newPos);
    //    }

    if (auto graphicsItemParent = parentItem()) {
        const QRectF parentRect = graphicsItemParent->sceneBoundingRect();
        setVisible(parentRect.contains(sceneBoundingRect()));
    }
    InteractiveObject::rebuildLayout();
    updateTextPosition();
}

void AADLFunctionTypeGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF &from,
                                                        const QPointF &to)
{
    Q_UNUSED(from)

    if (!scene() || grip != GripPoint::Location::Center || m_clickPos.isNull())
        return;

    QPointF newPos = mapToParent(mapFromScene(to) - m_clickPos);
    if (auto parentFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem())) {
        const QRectF contentRect = parentFunction->boundingRect().marginsRemoved(
                parentFunction->isRootItem() ? utils::kRootMargins : utils::kContentMargins);

        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
            newPos.setX(contentRect.right() - m_boundingRect.width());

        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
            newPos.setY(contentRect.bottom() - m_boundingRect.height());
    }
    setPos(newPos);

    //    rebuildLayout();
    updateGripPoints();

    //    Q_EMIT needUpdateLayout();
}

void AADLFunctionTypeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? m_selectedPen : pen());
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
    Q_UNUSED(grip)

    if (handlePositionChanged(pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionTypeGraphicsItem::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                        const QPointF &releasedAt)
{
    if (handleGeometryChanged(grip, pressedAt, releasedAt))
        createCommand();
}

void AADLFunctionTypeGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from,
                                                          const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());
    auto parentFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem());
    const QRectF contentRect = parentFunction ? parentFunction->boundingRect().marginsRemoved(
                                       parentFunction->isRootItem() ? utils::kRootMargins : utils::kContentMargins)
                                              : QRectF();
    switch (grip) {
    case GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        if (contentRect.isNull() || left >= contentRect.left())
            rect.setLeft(left);
    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        if (contentRect.isNull() || top >= contentRect.top())
            rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        if (contentRect.isNull() || right <= contentRect.right())
            rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        if (contentRect.isNull() || bottom <= contentRect.bottom())
            rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(topLeft))
            rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        if (contentRect.isNull() || contentRect.contains(topRight))
            rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomLeft))
            rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomRight))
            rect.setBottomRight(bottomRight);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    const QRectF normalized = rect.normalized();
    if (normalized.width() >= minimalSize().width() && normalized.height() >= minimalSize().height())
        setRect(parentItem() ? parentItem()->mapRectToScene(normalized) : normalized);

    //    rebuildLayout();
    updateGripPoints();

    //    Q_EMIT needUpdateLayout();
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
    const QSizeF textSize = m_textItem->boundingRect().size();
    return { qMax(textSize.width(), utils::DefaultGraphicsItemSize.width()),
             qMax(textSize.height(), utils::DefaultGraphicsItemSize.height()) };
}

void AADLFunctionTypeGraphicsItem::updateTextPosition()
{
    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    QRectF textRect { 0, 0, m_textItem->boundingRect().width() / currScale.x(),
                      m_textItem->boundingRect().height() / currScale.y() };
    textRect.moveTopLeft(boundingRect().marginsRemoved(utils::kTextMargins).topLeft());
    m_textItem->setPos(textRect.topLeft());
}

ColorManager::HandledColors AADLFunctionTypeGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::FunctionType;
}

AADLObject *AADLFunctionTypeGraphicsItem::aadlObject() const
{
    return entity();
}

void AADLFunctionTypeGraphicsItem::colorSchemeUpdated()
{
    const ColorHandler &h = colorHandler();
    setPen(h.pen());
    setBrush(h.brush());
    update();
}

} // namespace aadl
} // namespace taste3
