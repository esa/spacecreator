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

#include "aadlcommentgraphicsitem.h"

#include "aadlfunctiongraphicsitem.h"
#include "baseitems/common/utils.h"
#include "baseitems/textgraphicsitem.h"
#include "colors/colormanager.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QTextDocument>
#include <QtDebug>
#include <app/commandsstack.h>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace taste3 {
namespace aadl {

AADLCommentGraphicsItem::AADLCommentGraphicsItem(AADLObjectComment *comment, QGraphicsItem *parent)
    : InteractiveObject(comment, parent)
    , m_textItem(new TextGraphicsItem(this))
{
    setObjectName(QLatin1String("AADLCommentGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setPlainText(comment->title());
    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable);

    connect(m_textItem, &TextGraphicsItem::edited, this, &AADLCommentGraphicsItem::textEdited);
    connect(m_textItem, &TextGraphicsItem::textChanged, this, &AADLCommentGraphicsItem::textChanged);

    setFont(QFont(qApp->font()));
    m_textItem->setFont(font());

    setHighlightable(false);
    colorSchemeUpdated();
}

AADLObjectComment *AADLCommentGraphicsItem::entity() const
{
    return qobject_cast<AADLObjectComment *>(dataObject());
}

void AADLCommentGraphicsItem::updateFromEntity()
{
    aadl::AADLObjectComment *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    setText(obj->title());
    const QRectF itemSceneRect { utils::rect(obj->coordinates()) };
    if (!itemSceneRect.isValid())
        instantLayoutUpdate();
    else
        setRect(itemSceneRect);
}

void AADLCommentGraphicsItem::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    InteractiveObject::onManualResizeProgress(grip, from, to);

    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());
    AADLFunctionGraphicsItem *parentFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem());
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

    rect = rect.normalized();
    m_textItem->setExplicitSize(rect.size());
    const QSizeF &docSize = m_textItem->document()->size();
    const QSizeF &itemSize = m_textItem->boundingRect().size();
    if (docSize.width() > itemSize.width() || docSize.height() > itemSize.height())
        m_textItem->setExplicitSize(m_boundingRect.size());
    else
        setRect(rect);
}

void AADLCommentGraphicsItem::textEdited(const QString &text)
{
    if (entity()->title() == m_textItem->toPlainText())
        return;

    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change comment"));

    const QRectF geometry = sceneBoundingRect();
    const QVector<QPointF> points { geometry.topLeft(), geometry.bottomRight() };
    const QVariantList geometryParams { QVariant::fromValue(entity()), QVariant::fromValue(points) };
    const auto geometryCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, geometryParams);
    taste3::cmd::CommandsStack::current()->push(geometryCmd);

    const QVariantList commentTextParams { QVariant::fromValue(entity()), QVariant::fromValue(text) };
    const auto commentTextCmd = cmd::CommandsFactory::create(cmd::ChangeCommentText, commentTextParams);
    taste3::cmd::CommandsStack::current()->push(commentTextCmd);

    taste3::cmd::CommandsStack::current()->endMacro();
}

void AADLCommentGraphicsItem::textChanged()
{
    prepareGeometryChange();
    m_boundingRect = m_textItem->boundingRect();
    updateGripPoints();
}

void AADLCommentGraphicsItem::setText(const QString &text)
{
    if (m_textItem->toPlainText() == text)
        return;

    m_textItem->setTextWidth(150);
    m_textItem->setPlainText(text);
    m_textItem->setTextWidth(m_textItem->idealWidth());

    instantLayoutUpdate();
}

QString AADLCommentGraphicsItem::text() const
{
    return m_textItem->toPlainText();
}

void AADLCommentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());

    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    auto preparePolygon = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() - QPointF(kMargins, 0),
                                  rect.topLeft(),
                                  rect.bottomLeft(),
                                  rect.bottomRight(),
                                  rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolygon(preparePolygon(br));

    auto preparePolyline = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, -kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolyline(preparePolyline(br));

    painter->restore();
    InteractiveObject::paint(painter, option, widget);
}

void AADLCommentGraphicsItem::rebuildLayout()
{
    if (auto graphicsItemParent = parentItem()) {
        const QRectF parentRect = graphicsItemParent->sceneBoundingRect();
        setVisible(parentRect.contains(sceneBoundingRect()));
    }
    m_textItem->setExplicitSize(m_boundingRect.size());
}

void AADLCommentGraphicsItem::onManualMoveProgress(GripPoint::Location grip, const QPointF & /*from*/,
                                                   const QPointF &to)
{
    if (!scene() || grip != GripPoint::Location::Center || m_clickPos.isNull())
        return;

    QPointF newPos = mapToParent(mapFromScene(to) - m_clickPos);
    if (parentItem()) {
        AADLFunctionGraphicsItem *parentFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(parentItem());
        const QRectF contentRect = parentFunction ? parentFunction->boundingRect().marginsRemoved(
                                           parentFunction->isRootItem() ? utils::kRootMargins : utils::kContentMargins)
                                                  : QRectF();

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
    updateGripPoints();
}

void AADLCommentGraphicsItem::onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                 const QPointF &releasedAt)
{
    Q_UNUSED(grip)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)

    createCommand();
}

void AADLCommentGraphicsItem::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                                   const QPointF &releasedAt)
{
    Q_UNUSED(grip)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)

    createCommand();
}

QSizeF AADLCommentGraphicsItem::minimalSize() const
{
    return utils::DefaultGraphicsItemSize;
}

void AADLCommentGraphicsItem::createCommand()
{
    const QRectF geometry = sceneBoundingRect();
    const QVector<QPointF> points { geometry.topLeft(), geometry.bottomRight() };
    const auto geometryCmd = cmd::CommandsFactory::create(
            cmd::ChangeEntityGeometry, { QVariant::fromValue(entity()), QVariant::fromValue(points) });
    taste3::cmd::CommandsStack::current()->push(geometryCmd);
}

ColorManager::HandledColors AADLCommentGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Comment;
}

AADLObject *AADLCommentGraphicsItem::aadlObject() const
{
    return entity();
}

void AADLCommentGraphicsItem::colorSchemeUpdated()
{
    const ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

} // namespace aadl
} // namespace taste3
