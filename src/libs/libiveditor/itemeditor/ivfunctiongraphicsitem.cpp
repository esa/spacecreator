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
#include "ui/minimizelimits.h"
#include "ui/textitem.h"
#include "ui/resizelimits.h"

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

IVFunctionGraphicsItem::~IVFunctionGraphicsItem() { }

void IVFunctionGraphicsItem::init()
{
    IVFunctionTypeGraphicsItem::init();
    if (auto dataObj = entity()) {
        connect(dataObj, &ivm::IVFunction::childAdded, this, [this]() { update(); });
        connect(dataObj, &ivm::IVFunction::childRemoved, this, [this]() { update(); });
    }
}

shared::ui::ResizeLimits IVFunctionGraphicsItem::resizedRectForNestedFunctions(
        shared::ui::ResizeLimits resizeLimits) const
{
    // Calculate the limits of movement for each grip-point on a rect with no connections
    QRectF result = sceneBoundingRect();
    shared::ui::MinimizeLimits::Limits limits;
    limits.leftGripsRightMostLimit =
            result.right(); // The furthes to the right, the left-grip can go is the right side of the rect.
    limits.rightGripsLeftMostLimit =
            result.left(); // The furthest to the left, the right-grip can go is the left side of the rect.
    limits.topGripsBottomMostLimit = result.bottom(); // The deepest the top-grip can go, is the bottom of the rect.
    limits.bottomGripsTopMostLimit = result.top(); // The highest the bottom-grip can go, is the top of the rect.

    // The limits are determined by the bounding rect of the nested functions, so we calculated that
    QRectF nestedItemsRect = boundingRectForNestedFunctions();
    if (nestedItemsRect.isValid()) {
        limits.leftGripsRightMostLimit =
                nestedItemsRect.left(); // The furthest to the right, the left-grip can go is the left side of the
                                        // bounding rect for the nested functions.
        limits.rightGripsLeftMostLimit =
                nestedItemsRect.right(); // The furthest to the left, the right-grip can go is the right side of the
                                         // bounding rect for the nested functions.
        limits.topGripsBottomMostLimit = nestedItemsRect.top(); // The deepest the top-grip can go, is the top of the
                                                                // bounding rect for the nested functions.
        limits.bottomGripsTopMostLimit = nestedItemsRect.bottom(); // The highest the bottom-grip can go, is the bottom
                                                                   // of the bounding rect for the nested functions.
    }

    if (resizeLimits.isTopEdgeMinimizing()) {
        if (resizeLimits.isHorizontalLimitSet()) {
            qreal hLimit = resizeLimits.getHorizontalLimit();
            hLimit = qMin(hLimit, limits.topGripsBottomMostLimit);
            resizeLimits.setHorizontalLimit(hLimit);
        } else {
            resizeLimits.setHorizontalLimit(limits.topGripsBottomMostLimit);
        }
    }

    if (resizeLimits.isBottomEdgeMinimizing()) {
        if (resizeLimits.isHorizontalLimitSet()) {
            qreal hLimit = resizeLimits.getHorizontalLimit();
            hLimit = qMax(hLimit, limits.bottomGripsTopMostLimit);
            resizeLimits.setHorizontalLimit(hLimit);
        } else {
            resizeLimits.setHorizontalLimit(limits.bottomGripsTopMostLimit);
        }
    }

    if (resizeLimits.isLeftEdgeMinimizing()) {
        if (resizeLimits.isVerticalLimitSet()) {
            qreal vLimit = resizeLimits.getVerticalLimit();
            vLimit = qMin(vLimit, limits.leftGripsRightMostLimit);
            resizeLimits.setVerticalLimit(vLimit);
        } else {
            resizeLimits.setVerticalLimit(limits.leftGripsRightMostLimit);
        }
    }

    if (resizeLimits.isRightEdgeMinimizing()) {
        if (resizeLimits.isVerticalLimitSet()) {
            qreal vLimit = resizeLimits.getVerticalLimit();
            vLimit = qMax(vLimit, limits.rightGripsLeftMostLimit);
            resizeLimits.setVerticalLimit(vLimit);
        } else {
            resizeLimits.setVerticalLimit(limits.rightGripsLeftMostLimit);
        }
    }

    return resizeLimits;
}

//QRectF IVFunctionGraphicsItem::resizedRectForNestedFunctions(GripPoint *grip, const QPointF &from, const QPointF &to, const QRectF &rect) const
//{
//    // Calculate the limits of movement for each grip-point on a rect with no connections
//    QRectF result = rect;
//    shared::ui::MinimizeLimits::Limits limits;
//    limits.leftGripsRightMostLimit = result.right();  // The furthes to the right, the left-grip can go is the right side of the rect.
//    limits.rightGripsLeftMostLimit = result.left();   // The furthest to the left, the right-grip can go is the left side of the rect.
//    limits.topGripsBottomMostLimit = result.bottom(); // The deepest the top-grip can go, is the bottom of the rect.
//    limits.bottomGripsTopMostLimit = result.top();    // The highest the bottom-grip can go, is the top of the rect.

//    // The limits are determined by the bounding rect of the nested functions, so we calculated that
//    QRectF nestedItemsRect = boundingRectForNestedFunctions();
//    if (nestedItemsRect.isValid())
//    {
//        limits.leftGripsRightMostLimit = nestedItemsRect.left(); // The furthest to the right, the left-grip can go is the left side of the bounding rect for the nested functions.
//        limits.rightGripsLeftMostLimit = nestedItemsRect.right(); // The furthest to the left, the right-grip can go is the right side of the bounding rect for the nested functions.
//        limits.topGripsBottomMostLimit = nestedItemsRect.top(); // The deepest the top-grip can go, is the top of the bounding rect for the nested functions.
//        limits.bottomGripsTopMostLimit = nestedItemsRect.bottom(); // The highest the bottom-grip can go, is the bottom of the bounding rect for the nested functions.
//    }
//    // Apply the movement described by 'to' while respecting the calculated limits
//    result = shared::ui::MinimizeLimits::applyTo(result, limits, grip->location(), from, to);
//    return result;
//}

QRectF IVFunctionGraphicsItem::boundingRectForNestedFunctions() const
{
    QRectF nestedItemsBoundingRect; // Start with empty rectangle
    for (const QGraphicsItem *item : childItems())
    {
        auto rectItem = qobject_cast<const shared::ui::VERectGraphicsItem *>(item->toGraphicsObject());
        if (rectItem == nullptr)
        {
            continue;
        }
        const QRectF nestedRect = rectItem->sceneBoundingRect();
        if (!nestedRect.isValid())
        {
            continue;
        }

        if (nestedItemsBoundingRect.isEmpty())
        {
            nestedItemsBoundingRect = nestedRect;
        }
        else
        {
            nestedItemsBoundingRect = nestedItemsBoundingRect.united(nestedRect);
        }

    }
    // If nestedItemsBoundingRect is empty it must stay empty, so don't add margins
    if (!nestedItemsBoundingRect.isEmpty())
    {
        nestedItemsBoundingRect = nestedItemsBoundingRect.marginsAdded(shared::graphicsviewutils::kTextMargins);
    }
    return nestedItemsBoundingRect;
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

QRectF IVFunctionGraphicsItem::resizedRect(shared::ui::ResizeLimits resizeLimits)
{
    if (!isRootItem()) {
        resizeLimits = resizeLimitsForCollision(resizeLimits);
    }
    resizeLimits = resizeRectForConnectionEndpoints(resizeLimits);
    resizeLimits = resizedRectForTextLabel(resizeLimits);
    resizeLimits = resizedRectForNestedFunctions(resizeLimits);
    QRectF limitedRect = resizeLimits.getLimitedRect();
    return limitedRect;
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

shared::ColorManager::HandledColors IVFunctionGraphicsItem::handledColorType() const
{
    if (isRootItem())
        return shared::ColorManager::HandledColors::FunctionRoot;

    const QRectF nestedRect = nestedItemsSceneBoundingRect();
    if (nestedRect.isValid()
            && !sceneBoundingRect().contains(nestedRect.marginsAdded(shared::graphicsviewutils::kContentMargins)))
        return shared::ColorManager::HandledColors::FunctionPartial;

    return entity()->isMarked() ? shared::ColorManager::HandledColors::FunctionHighlighted
                                : shared::ColorManager::HandledColors::FunctionRegular;
}

shared::ui::TextItem *IVFunctionGraphicsItem::initTextItem()
{
    auto textItem = IVFunctionTypeGraphicsItem::initTextItem();
    textItem->setVisible(!isRootItem());
    textItem->setTextAlignment(Qt::AlignCenter);
    return textItem;
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
