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
#include "ivfunction.h"
#include "ivfunctionnamegraphicsitem.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "parameter.h"
#include "ui/textitem.h"
#include "ui/grippoint.h"
#include "veobject.h"
#include "ivcoreutils.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPainter>
#include <QTextDocument>
#include <QtDebug>

static const qreal kBorderWidth = 2;

namespace ive {

using shared::ui::GripPoint;

IVFunctionTypeGraphicsItem::IVFunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(entity, parent)
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
    setVisible(entity() && (ivm::utils::nestingLevel(entity()) >= gi::kNestingVisibilityLevel || entity()->isRootObject())
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
    const QSizeF txtSize = m_textItem->document()->size();
    bool labelBiggerThanBox = txtSize.width() > maxTxtSize.width() || txtSize.height() > maxTxtSize.height();
    if (labelBiggerThanBox)
    {
        m_textItem->setExplicitSize(maxTxtSize);
    }

    // place the text item at top-left of this instance's bounding box
    alignTextItem();
}

shared::ColorManager::HandledColors IVFunctionTypeGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::FunctionType;
}

void IVFunctionTypeGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    setPen(h.pen());
    setBrush(h.brush());
    update();
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

QRectF IVFunctionTypeGraphicsItem::resizedRect(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    // The parent class VERectGraphicsItem calculates resized rect to respect the interfaces
    QRectF interfaceRect = shared::ui::VERectGraphicsItem::resizedRect(grip, from, to);

    // This class has a label in the center of the rectangle that should always be readable.
    QRectF textLabelRect = resizedRectForTextLabel(grip, from, to);

    auto unionRect = interfaceRect.united(textLabelRect);
    return unionRect;
}

QRectF IVFunctionTypeGraphicsItem::resizedRectForTextLabel(GripPoint *grip, const QPointF &from, const QPointF &to) const
{
    QRectF result = sceneBoundingRect();
    QRectF textLabelRect = m_textItem->boundingRect();
    auto text = m_textItem->toPlainText();
    textLabelRect = textLabelRect.marginsAdded(shared::graphicsviewutils::kTextMargins);
    textLabelRect.setWidth(textLabelRect.width() + 5.0); // A little extra space is needed, otherwise the text item moves about ever so sligtly.
    QSizeF minSize = textLabelRect.size();

    // The minimum x-value the right side of this rect can have and not violate minimum size
    auto xMin = result.left() + minSize.width();
    // The maximum x-value the left side of this rect can have and not violate minimum size
    auto xMax = result.right() - minSize.width();
    // The minimum y-value the top side of this rect can have and not violate minimum size
    auto yMin = result.top() + minSize.height();
    // The maximum y-value the buttom side of this rect can have and not violate minimum size
    auto yMax = result.bottom() - minSize.height();

    switch (grip->location()) {
    case GripPoint::Left:
    {
        result.setLeft(qMin(to.x(), xMax));
        break;
    }
    case GripPoint::Right:
    {
        result.setRight(qMax(to.x(), xMin));
        break;
    }
    case GripPoint::Top:
    {
        result.setTop(qMin(to.y(), yMax));
        break;
    }
    case GripPoint::Bottom:
    {
        result.setBottom(qMax(to.y(), yMin));
        break;
    }
    case GripPoint::TopLeft:
    {
        result.setTopLeft(QPoint(qMin(to.x(), xMax), qMin(to.y(), yMax)));
        break;
    }
    case GripPoint::TopRight:
    {
        result.setTopRight(QPoint(qMax(to.x(), xMin), qMin(to.y(), yMax)));
        break;
    }
    case GripPoint::BottomLeft:
    {
        result.setBottomLeft(QPoint(qMin(to.x(), xMax), qMax(to.y(), yMin)));
        break;
    }
    case GripPoint::BottomRight:
    {
        result.setBottomRight(QPoint(qMax(to.x(), xMin), qMax(to.y(), yMin)));
        break;
    }
    default:
        qWarning() << "Update grip point handling";
        break;
    }
    return result;
}


bool IVFunctionTypeGraphicsItem::isRootItem() const
{
    return !parentItem() && entity() && entity()->isRootObject();
}

void IVFunctionTypeGraphicsItem::alignTextItem() const
{
    auto myRectWithoutMargins = boundingRect().marginsRemoved(shared::graphicsviewutils::kTextMargins);
    auto myTopLeft = myRectWithoutMargins.topLeft();
    QRectF textRect = m_textItem->boundingRect();
    textRect.moveTopLeft(myTopLeft);
    m_textItem->setPos(textRect.topLeft());
}


shared::ui::TextItem *IVFunctionTypeGraphicsItem::initTextItem()
{
    shared::ui::TextItem *textItem = new IVFunctionNameGraphicsItem(this);

    // Connect 'clicked' with 'enableEditMode' on clicked text item.
    connect(this, &IVFunctionTypeGraphicsItem::clicked, textItem,
            [this, textItem](const QPointF &scenePos)
            {
                if (scene()->items(scenePos).contains(textItem))
                    textItem->enableEditMode();
            });

    // Connect 'edited' with 'updateNameFromUi' to keep model updated when UI changes.
    connect(textItem, &shared::ui::TextItem::edited, this, &IVFunctionTypeGraphicsItem::updateNameFromUi);

    // Connect 'attributeChanged' with 'updateText' to keep UI updated when model changes.
    connect(entity(), &ivm::IVFunction::attributeChanged, this,
            [this](const QString &token)
            {
                const ivm::meta::Props::Token attr = ivm::meta::Props::token(token);
                if (attr == ivm::meta::Props::Token::name || attr == ivm::meta::Props::Token::url)
                {
                    updateText();
                }
            });

    textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    textItem->setFont(font());
    return textItem;
}

}
