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

#include "ivfunctiongraphicsitem.h"

#include "colors/colormanager.h"
#include "graphicsviewutils.h"
#include "ivfunction.h"
#include "miniviewrenderer.h"
#include "ui/textitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QtDebug>
#include <cmath>

static const qreal kBorderWidth = 2.0;
static const qreal kRadius = 10.0;
static const qreal kOffset = kBorderWidth / 2.0;

namespace ive {

QPointer<QSvgRenderer> IVFunctionGraphicsItem::m_svgRenderer = {};

IVFunctionGraphicsItem::IVFunctionGraphicsItem(ivm::IVFunction *entity, QGraphicsItem *parent)
    : IVFunctionTypeGraphicsItem(entity, parent)
    , m_miniViewDrawer(std::make_unique<MiniViewRenderer>(this))
{
    if (!m_svgRenderer) // TODO: change icon
        m_svgRenderer = new QSvgRenderer(QLatin1String(":/toolbar/icns/change_root.svg"));
}

IVFunctionGraphicsItem::~IVFunctionGraphicsItem() {}

void IVFunctionGraphicsItem::init()
{
    IVFunctionTypeGraphicsItem::init();
    if (auto dataObj = entity()) {
        connect(dataObj, &ivm::IVFunction::childAdded, this, [this]() { update(); });
        connect(dataObj, &ivm::IVFunction::childRemoved, this, [this]() { update(); });
    }
}

ivm::IVFunction *IVFunctionGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVFunction *>(m_dataObject);
}

QPainterPath IVFunctionGraphicsItem::shape() const
{
    QPainterPath pp;
    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        pp.addRect(br);
    else
        pp.addRoundedRect(br, kRadius, kRadius);
    return pp;
}

void IVFunctionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? selectedPen() : m_pen);
    painter->setBrush(brush());

    const QRectF br = boundingRect().adjusted(kOffset, kOffset, -kOffset, -kOffset);
    if (isRootItem())
        painter->drawRect(br);
    else
        painter->drawRoundedRect(br, kRadius, kRadius);

    if (!isRootItem() && entity() && entity()->hasNestedChildren()) {
        m_miniViewDrawer->render(painter);
        QRectF iconRect { QPointF(0, 0), m_svgRenderer->defaultSize() };
        iconRect.moveTopRight(br.adjusted(kRadius, kRadius, -kRadius, -kRadius).topRight());
        m_svgRenderer->render(painter, iconRect);
    }

    painter->restore();
}

void IVFunctionGraphicsItem::onManualMoveProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (isRootItem())
        return;

    IVFunctionTypeGraphicsItem::onManualMoveProgress(grip, pressedAt, releasedAt);
}

void IVFunctionGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (isRootItem())
        return;

    IVFunctionTypeGraphicsItem::onManualMoveFinish(grip, pressedAt, releasedAt);
}

void IVFunctionGraphicsItem::alignTextItem() const
{
    QPointF myCenter = boundingRect().center();
    QRectF textRect = m_textItem->boundingRect();
    textRect.moveCenter(myCenter);
    m_textItem->setPos(textRect.topLeft());
}

shared::ColorManager::HandledColors IVFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return shared::ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid()
            && !sceneBoundingRect().contains(nestedRect.marginsAdded(shared::graphicsviewutils::kContentMargins)))
        return shared::ColorManager::HandledColors::FunctionPartial;

    return shared::ColorManager::HandledColors::FunctionRegular;
}

shared::ui::TextItem *IVFunctionGraphicsItem::initTextItem()
{
    auto textItem = IVFunctionTypeGraphicsItem::initTextItem();
    textItem->setVisible(!isRootItem());
    textItem->setTextAlignment(Qt::AlignCenter);
    return textItem;
}

void IVFunctionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen p = h.pen();
    QBrush b = h.brush();

    if (auto parentFunction = qgraphicsitem_cast<IVFunctionGraphicsItem *>(parentItem())) {
        if (!parentFunction->entity()->hasEntityAttribute(QLatin1String("color"))
                && !entity()->hasEntityAttribute(QLatin1String("color"))
                && parentFunction->handledColorType()
                        == shared::ColorManager::HandledColors::FunctionRegular) { // [Hm...]
            b.setColor(parentFunction->brush().color().darker(125));
            p.setColor(parentFunction->pen().color().darker(125));
        }
    }

    if (pen() == p && brush() == b)
        return;

    setPen(p);
    setBrush(b);

    // During undo, a child can be updated before its parent,
    // so on the step marked as [Hm...] above, the parent is still of type FunctionPartial and not the
    // FunctionRegular. Thus, the child gets the "default" colour, instead of "parent.darker". For now, I can't see
    // a better way but just to update children colours manually:
    for (auto child : childItems())
        if (child->type() == IVFunctionGraphicsItem::Type)
            if (auto nestedFunction = qobject_cast<IVFunctionGraphicsItem *>(child->toGraphicsObject()))
                nestedFunction->applyColorScheme();

    update();
}

IVFunctionGraphicsItem::IVFunctionGraphicsItem(
        ivm::IVFunction *entity, std::unique_ptr<MiniViewRenderer> renderer, QGraphicsItem *parent)
    : IVFunctionTypeGraphicsItem(entity, parent)
    , m_miniViewDrawer(std::move(renderer))
{
    if (!m_svgRenderer) // TODO: change icon
        m_svgRenderer = new QSvgRenderer(QLatin1String(":/toolbar/icns/change_root.svg"));
}

} // namespace ive
