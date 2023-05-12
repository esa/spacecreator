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

#include "ivfunctiontypegraphicsitem.h"

#include "colors/colormanager.h"
#include "commands/cmdentityattributeschange.h"
#include "commands/cmdfunctionattrchange.h"
#include "commandsstackbase.h"
#include "common.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "ivcommonprops.h"
#include "ivcoreutils.h"
#include "ivfunction.h"
#include "ivfunctionnamegraphicsitem.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "parameter.h"
#include "ui/grippoint.h"
#include "ui/textitem.h"
#include "veobject.h"
#include "graphicsviewutils.h"
#include "ui/resizelimits.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPainter>
#include <QStyle>
#include <QTextDocument>
#include <QtDebug>

static const qreal kBorderWidth = 2;
static const qreal extraSpace = 5.0;

namespace ive {

using shared::ui::GripPoint;
using shared::graphicsviewutils::kContentMargins;
using shared::ui::ResizeType;
using shared::ui::VerticalEdge;
using shared::ui::HorizontalEdge;
using shared::ui::VERectGraphicsItem;

IVFunctionTypeGraphicsItem::IVFunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parent)
    : VERectGraphicsItem(entity, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ZOrder.Function);
}

ivm::IVFunctionType *IVFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVFunctionType *>(m_dataObject);
}

void IVFunctionTypeGraphicsItem::enableEditMode()
{
    m_textItem->enableEditMode();
}

void IVFunctionTypeGraphicsItem::rebuildLayout()
{
    shared::ui::VERectGraphicsItem::rebuildLayout();
    setVisible(entity()
            && (ivm::utils::nestingLevel(entity()) >= gi::kNestingVisibilityLevel || entity()->isRootObject())
            && entity()->isVisible());

    updateTextPosition();
}

int IVFunctionTypeGraphicsItem::itemLevel(bool isSelected) const
{
    return gi::itemLevel(entity(), isSelected);
}

void IVFunctionTypeGraphicsItem::init()
{
    shared::ui::VERectGraphicsItem::init();
    updateText();
}

void IVFunctionTypeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected() ? selectedPen() : pen());
    painter->setBrush(brush());
    QRectF rect =
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2).toRect();
    QPainterPath path = shared::graphicsviewutils::edgeCuttedRectShape(rect, 6);
    painter->fillPath(path, brush());
    painter->drawPath(path);

    qreal offset = 3 + painter->pen().width();
    rect = rect.adjusted(offset, offset, -offset, -offset);
    path = shared::graphicsviewutils::edgeCuttedRectShape(rect, 5);
    painter->drawPath(path);

    painter->restore();
}

QString IVFunctionTypeGraphicsItem::prepareTooltip() const
{
    const QString title = shared::uniteNames<ivm::IVFunctionType *>({ entity() }, QString());
    const QString instances = shared::uniteNames<QPointer<ivm::IVFunction>>(entity()->instances(), tr("Instances: "));
    const QString ris = shared::uniteNames<ivm::IVInterface *>(entity()->ris(), tr("RI: "));
    const QString pis = shared::uniteNames<ivm::IVInterface *>(entity()->pis(), tr("PI: "));
    const QString ctxParamsStr = shared::uniteNames(entity()->contextParams(), tr("Parameters: "));

    return shared::joinNonEmpty({ title, instances, ris, pis, ctxParamsStr }, QStringLiteral("<br>"));
}

void IVFunctionTypeGraphicsItem::updateTextPosition()
{
    // To an IVFunctionTypeGraphicsItem an m_textItem is the name label in the center of the box
    if (!m_textItem) {
        return;
    }
    const QRectF targetTextRect = boundingRect().marginsRemoved(shared::graphicsviewutils::kTextMargins);

    // reset m_textItems's size
    m_textItem->setExplicitSize(QSizeF());
    m_textItem->setTextWidth(-1);

    // Set text width so that it does not exeed the boundaries of the this instance's bounding box
    const QSizeF maxTxtSize = targetTextRect.size();
    QSizeF txtSize = m_textItem->document()->size();
    bool labelBiggerThanBox = txtSize.width() > maxTxtSize.width() || txtSize.height() > maxTxtSize.height();
    if (labelBiggerThanBox) {
        txtSize = maxTxtSize;
    }
    m_textItem->setExplicitSize(txtSize);

    auto itemRect = QStyle::alignedRect(Qt::LayoutDirection::LayoutDirectionAuto, m_textItem->textAlignment(),
            txtSize.toSize(), targetTextRect.toAlignedRect());
    m_textItem->setPos(itemRect.topLeft());
}

shared::ColorManager::HandledColors IVFunctionTypeGraphicsItem::handledColorType() const
{
    return entity()->isMarked() ? shared::ColorManager::HandledColors::FunctionTypeHighlighted
                                : shared::ColorManager::HandledColors::FunctionType;
}

void IVFunctionTypeGraphicsItem::updateNameFromUi(const QString &name)
{
    if (name == entity()->titleUI()) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for IVFunctionTypeGraphicsItem";
        return;
    }

    const QString newName = ivm::IVNameValidator::encodeName(entity()->type(), name);
    if (!ivm::IVNameValidator::isAcceptableName(entity(), newName)) {
        updateText();
        return;
    }
    if (entity()->model()->nestedFunctionNames().contains(newName)) {
        updateText();
        return;
    }

    QUndoCommand *attributesCmd = nullptr;
    const QList<EntityAttribute> attributess = { EntityAttribute {
            ivm::meta::Props::token(ivm::meta::Props::Token::name), newName, EntityAttribute::Type::Attribute } };
    if (entity()->type() == ivm::IVObject::Type::Function) {
        attributesCmd = new ive::cmd::CmdFunctionAttrChange(
                ivm::IVPropertyTemplateConfig::instance(), entity()->as<ivm::IVFunction *>(), attributess);
    } else {
        attributesCmd = new shared::cmd::CmdEntityAttributesChange(
                ivm::IVPropertyTemplateConfig::instance(), entity(), attributess);
    }
    m_commandsStack->push(attributesCmd);
}

QRectF IVFunctionTypeGraphicsItem::resizedRect(shared::ui::ResizeLimits resizeLimits)
{
    resizeLimits = resizeLimitsForCollision(resizeLimits);
    resizeLimits = resizeRectForConnectionEndpoints(resizeLimits);
    resizeLimits = resizedRectForTextLabel(resizeLimits);
    QRectF limitedRect = resizeLimits.getLimitedRect();
    return limitedRect;
}

qreal findLeftMostEdge(const QList<VERectGraphicsItem *> &collidingItems)
{
    // Find the left-most edge of the colliding items
    auto leftMostItemIter = std::min_element(collidingItems.begin(), collidingItems.end(), [](VERectGraphicsItem *a, VERectGraphicsItem *b)
                                             {
                                                 return a->sceneBoundingRect().left() < b->sceneBoundingRect().left();
                                             });
    qreal leftMostEdge = (*leftMostItemIter)->sceneBoundingRect().left();
    return leftMostEdge;
}

qreal findRightMostEdge(const QList<VERectGraphicsItem *> &collidingItems)
{
    // Find the right-most edge of the colliding items
    auto rightMostItemIter = std::max_element(collidingItems.begin(), collidingItems.end(), [](VERectGraphicsItem *a, VERectGraphicsItem *b)
                                              {
                                                  return a->sceneBoundingRect().right() < b->sceneBoundingRect().right();
                                              });
    qreal rightMostEdge = (*rightMostItemIter)->sceneBoundingRect().right();
    return rightMostEdge;
}

qreal findTopMostEdge(const QList<VERectGraphicsItem *> &collidingItems)
{
    // Find the top-most edge of the colliding items
    auto topItemIter = std::min_element(collidingItems.begin(), collidingItems.end(), [](VERectGraphicsItem *a, VERectGraphicsItem *b)
                                        {
                                            return a->sceneBoundingRect().top() < b->sceneBoundingRect().top();
                                        });
    qreal topEdge = (*topItemIter)->sceneBoundingRect().top();
    return topEdge;
}

qreal findBottomMostEdge(const QList<VERectGraphicsItem *> &collidingItems)
{
    // Find the right-most edge of the colliding items
    auto lowestItemIter = std::max_element(collidingItems.begin(), collidingItems.end(), [](VERectGraphicsItem *a, VERectGraphicsItem *b)
                                           {
                                               return a->sceneBoundingRect().bottom() < b->sceneBoundingRect().bottom();
                                           });
    qreal lowestEdge = (*lowestItemIter)->sceneBoundingRect().bottom();
    return lowestEdge;
}

shared::ui::ResizeLimits IVFunctionTypeGraphicsItem::resizeLimitsForCollision(shared::ui::ResizeLimits resizeLimits)
{
    // In case this VERectGraphicsItem has a parent (is a nested function) check
    // for collision with the parent's edges
    QGraphicsItem *parent = parentItem();
    VERectGraphicsItem *parentVERectGraphicsItem = nullptr;
    if (parent) {
        parentVERectGraphicsItem = qobject_cast<VERectGraphicsItem *>(parent->toGraphicsObject());
    }
    if (parentVERectGraphicsItem) {
        QRectF parentRect = parentVERectGraphicsItem->sceneBoundingRect();

        if (resizeLimits.isLeftEdgeMaximizing()) {
            // See if left edge of this rect is left of parent left edge
            qreal parentLeftEdge = parentRect.left();
            parentLeftEdge += kContentMargins.left();
            qreal unlimitedLeft = resizeLimits.getUnlimitedRect().left();
            if (parentLeftEdge > unlimitedLeft) {
                resizeLimits.setVerticalLimit(parentLeftEdge);
            }
        }

        if (resizeLimits.isRightEdgeMaximizing()) {
            // See if right edge of this rect is right of parent right edge
            qreal parentRightEdge = parentRect.right();
            parentRightEdge -= kContentMargins.right();
            qreal unlimitedRight = resizeLimits.getUnlimitedRect().right();
            if (parentRightEdge < unlimitedRight) {
                resizeLimits.setVerticalLimit(parentRightEdge);
            }
        }

        if (resizeLimits.isTopEdgeMaximizing()) {
            // See if top edge of this rect is above parent top edge
            qreal parentTopEdge = parentRect.top();
            parentTopEdge += kContentMargins.top();
            qreal unlimitedTop = resizeLimits.getUnlimitedRect().top();
            if (parentTopEdge > unlimitedTop) {
                resizeLimits.setHorizontalLimit(parentTopEdge);
            }
        }

        if (resizeLimits.isBottomEdgeMaximizing()) {
            // See if bottom edge of this rect is below parent bottom edge
            qreal parentBottomEdge = parentRect.bottom();
            parentBottomEdge -= kContentMargins.bottom();
            qreal unlimitedBottom = resizeLimits.getUnlimitedRect().bottom();
            if (parentBottomEdge < unlimitedBottom) {
                resizeLimits.setHorizontalLimit(parentBottomEdge);
            }
        }
    }


    bool bothResizeTypesAreMax = resizeLimits.getVerticalResizeType() == ResizeType::Max
                                 && resizeLimits.getHorizontalResizeType() == ResizeType::Max;
    if (bothResizeTypesAreMax) {
        VerticalEdge vEdge = resizeLimits.getVerticalEdge();
        HorizontalEdge hEdge = resizeLimits.getHorizontalEdge();
        bool areaIsTopLeft = hEdge == HorizontalEdge::TopEdge && vEdge == VerticalEdge::LeftEdge;
        bool areaIsTopRight = hEdge == HorizontalEdge::TopEdge && vEdge == VerticalEdge::RightEdge;
        bool areaIsBottomRight = hEdge == HorizontalEdge::BottomEdge && vEdge == VerticalEdge::RightEdge;
        bool areaIsBottomLeft = hEdge == HorizontalEdge::BottomEdge && vEdge == VerticalEdge::LeftEdge;

        QList<VERectGraphicsItem *> collidingItems;
        if (areaIsTopLeft) {
            QRectF topLeftRect = resizeLimits.getUnlimitedTopLeftRect();
            collidingItems = findCollidingVERectGraphicsItems(topLeftRect, QMarginsF(kContentMargins.left(), kContentMargins.top(), 0.0, 0.0));
        }
        if (areaIsTopRight) {
            QRectF topRightRect = resizeLimits.getUnlimitedTopRightRect();
            collidingItems = findCollidingVERectGraphicsItems(topRightRect, QMarginsF(0.0, kContentMargins.top(), kContentMargins.right(), 0.0));
        }
        if (areaIsBottomRight) {
            QRectF bottomRight = resizeLimits.getUnlimitedBottomRightRect();
            collidingItems = findCollidingVERectGraphicsItems(bottomRight, QMarginsF(0.0, 0.0, kContentMargins.right(), kContentMargins.bottom()));
        }
        if (areaIsBottomLeft) {
            QRectF bottomLeftRect = resizeLimits.getUnlimitedBottomLeftRect();
            collidingItems = findCollidingVERectGraphicsItems(bottomLeftRect, QMarginsF(kContentMargins.left(), 0.0, 0.0, kContentMargins.bottom()));
        }

        if (!collidingItems.isEmpty()) {
            QRectF collidingItemRect = collidingItems.first()->sceneBoundingRect();
            QPair<HorizontalEdge, VerticalEdge> intersectedEdges = resizeLimits.getIntersectedEdges(collidingItemRect);
            HorizontalEdge intersectedHorizontalEdge = intersectedEdges.first;
            VerticalEdge intersectedVerticalEdge = intersectedEdges.second;

            if (intersectedHorizontalEdge == HorizontalEdge::TopEdge) {
                if (resizeLimits.isHorizontalLimitSet()) {
                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
                    resizeLimits.setHorizontalLimit(qMin(horizontalLimit, collidingItemRect.top()));
                } else {
                    resizeLimits.setHorizontalLimit(collidingItemRect.top());
                }
            }
            if (intersectedHorizontalEdge == HorizontalEdge::BottomEdge) {
                if (resizeLimits.isHorizontalLimitSet()) {
                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
                    resizeLimits.setHorizontalLimit(qMax(horizontalLimit, collidingItemRect.bottom()));
                } else {
                    resizeLimits.setHorizontalLimit(collidingItemRect.bottom());
                }
            }

            if (intersectedVerticalEdge == VerticalEdge::LeftEdge) {
                if (resizeLimits.isVerticalLimitSet()) {
                    qreal verticalLimit = resizeLimits.getVerticalLimit();
                    resizeLimits.setVerticalLimit(qMin(verticalLimit, collidingItemRect.left()));
                } else {
                    resizeLimits.setVerticalLimit(collidingItemRect.left());
                }
            }
            if (intersectedVerticalEdge == VerticalEdge::RightEdge) {
                if (resizeLimits.isVerticalLimitSet()) {
                    qreal verticalLimit = resizeLimits.getVerticalLimit();
                    resizeLimits.setVerticalLimit(qMax(verticalLimit, collidingItemRect.right()));
                } else {
                    resizeLimits.setVerticalLimit(collidingItemRect.right());
                }
            }
        }
    }

    // This section is for when the user drags the right or left grippoint in a maximizing manner
    bool rectIsLeftAndRight = resizeLimits.getVerticalResizeType() == ResizeType::Max;
    if (rectIsLeftAndRight) {
        // Detect collision left and right
        QRectF leftRightRect = resizeLimits.getUnlimitedVerticalRect(); // Get the unlimited rect as if the user had
            // only used the right-, or left-grippoint.
        QList<VERectGraphicsItem *> collidingItemsLeftOrRight = findCollidingVERectGraphicsItems(leftRightRect, QMarginsF(kContentMargins.left(), 0.0, kContentMargins.right(), 0.0));

        if (!collidingItemsLeftOrRight.isEmpty()) {
            if (resizeLimits.getVerticalEdge() == VerticalEdge::LeftEdge) {
                qreal rightMostEdge = findRightMostEdge(collidingItemsLeftOrRight);
                rightMostEdge += kContentMargins.left();
                if (resizeLimits.isVerticalLimitSet()) {
                    qreal verticalLimit = resizeLimits.getVerticalLimit();
                    resizeLimits.setVerticalLimit(qMax(verticalLimit, rightMostEdge));
                } else {
                    resizeLimits.setVerticalLimit(rightMostEdge);
                }
                resizeLimits.setVerticalLimit(rightMostEdge);
            }
            if (resizeLimits.getVerticalEdge() == VerticalEdge::RightEdge) {
                qreal leftMostEdge = findLeftMostEdge(collidingItemsLeftOrRight);
                leftMostEdge -= kContentMargins.right();
                if (resizeLimits.isVerticalLimitSet()) {
                    qreal verticalLimit = resizeLimits.getVerticalLimit();
                    resizeLimits.setVerticalLimit(qMin(verticalLimit, leftMostEdge));
                } else {
                    resizeLimits.setVerticalLimit(leftMostEdge);
                }
            }
        }
    }

    // This section is for when the user drags the top or bottom grippoint
    bool rectIsUpAndDown = resizeLimits.getHorizontalResizeType() == ResizeType::Max;
    if (rectIsUpAndDown) {
        // Detect collision up and down
        QRectF upDownRect = resizeLimits.getUnlimitedHorizontalRect(); // Get the unlimited rect as if the user had only
            // used the top-, or bottom-grippoint.
        QList<VERectGraphicsItem *> collidingItemsUpOrDown = findCollidingVERectGraphicsItems(upDownRect, QMarginsF(0.0, kContentMargins.top(), 0.0, kContentMargins.bottom()));

        if (!collidingItemsUpOrDown.isEmpty()) {
            if (resizeLimits.getHorizontalEdge() == HorizontalEdge::TopEdge) {
                qreal bottomMostEdge = findBottomMostEdge(collidingItemsUpOrDown);
                bottomMostEdge += kContentMargins.top();
                if (resizeLimits.isHorizontalLimitSet()) {
                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
                    resizeLimits.setHorizontalLimit(qMax(horizontalLimit, bottomMostEdge));
                } else {
                    resizeLimits.setHorizontalLimit(bottomMostEdge);
                }
            }
            if (resizeLimits.getHorizontalEdge() == HorizontalEdge::BottomEdge) {
                qreal topMostEdge = findTopMostEdge(collidingItemsUpOrDown);
                topMostEdge -= kContentMargins.bottom();
                if (resizeLimits.isHorizontalLimitSet()) {
                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
                    resizeLimits.setHorizontalLimit(qMin(horizontalLimit, topMostEdge));
                } else {
                    resizeLimits.setHorizontalLimit(topMostEdge);
                }
            }
        }
    }

    return resizeLimits;
}

shared::ui::ResizeLimits IVFunctionTypeGraphicsItem::resizedRectForTextLabel(shared::ui::ResizeLimits resizeLimits)
{
    QRectF textLabelRect = m_textItem->sceneBoundingRect();
    textLabelRect = textLabelRect.marginsAdded(shared::graphicsviewutils::kTextMargins);
    textLabelRect.setWidth(textLabelRect.width()
            + extraSpace); // A little extra space is needed, otherwise the text item moves about ever so sligtly.

    QRectF rect = sceneBoundingRect();
    if (resizeLimits.isTopEdgeMinimizing()) {
        qreal topGripsBottomMostLimit = rect.bottom() - textLabelRect.height();
        resizeLimits.setHorizontalLimit(qMin(topGripsBottomMostLimit, resizeLimits.getHorizontalLimit()));
    }
    if (resizeLimits.isBottomEdgeMinimizing()) {
        qreal bottomGripsTopMostLimit = rect.top() + textLabelRect.height();
        resizeLimits.setHorizontalLimit(qMax(bottomGripsTopMostLimit, resizeLimits.getHorizontalLimit()));
    }
    if (resizeLimits.isLeftEdgeMinimizing()) {
        qreal leftGripsRightMostLimit = rect.right() - textLabelRect.width();
        resizeLimits.setVerticalLimit(qMin(leftGripsRightMostLimit, resizeLimits.getVerticalLimit()));
    }
    if (resizeLimits.isRightEdgeMinimizing()) {
        qreal rightGripsLeftMostLimit = rect.left() + textLabelRect.width();
        resizeLimits.setVerticalLimit(qMax(rightGripsLeftMostLimit, resizeLimits.getVerticalLimit()));
    }
    return resizeLimits;
}



bool IVFunctionTypeGraphicsItem::isRootItem() const
{
    return !parentItem() && entity() && entity()->isRootObject();
}

shared::ui::TextItem *IVFunctionTypeGraphicsItem::initTextItem()
{
    shared::ui::TextItem *textItem = new IVFunctionNameGraphicsItem(this);

    // Connect 'clicked' with 'enableEditMode' on clicked text item.
    connect(this, &IVFunctionTypeGraphicsItem::clicked, textItem, [this, textItem](const QPointF &scenePos) {
        if (scene()->items(scenePos).contains(textItem))
            textItem->enableEditMode();
    });

    // Connect 'edited' with 'updateNameFromUi' to keep model updated when UI changes.
    connect(textItem, &shared::ui::TextItem::edited, this, &IVFunctionTypeGraphicsItem::updateNameFromUi);

    // Connect 'attributeChanged' with 'updateText' to keep UI updated when model changes.
    connect(entity(), &ivm::IVFunction::attributeChanged, this, [this](const QString &token) {
        const ivm::meta::Props::Token attr = ivm::meta::Props::token(token);
        if (attr == ivm::meta::Props::Token::name || attr == ivm::meta::Props::Token::url) {
            updateText();
        }
    });

    textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    textItem->setFont(font());
    textItem->setEditable(!entity()->isReference() && !entity()->isInstanceDescendant());
    return textItem;
}

}
