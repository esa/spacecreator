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

#include "aadlcommonprops.h"
#include "ivconnection.h"
#include "aadlconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "baseitems/common/aadlutils.h"
#include "colors/colormanager.h"
#include "commands/cmdentityattributechange.h"
#include "commandsstack.h"
#include "graphicsviewutils.h"
#include "ui/textitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPainter>
#include <QtDebug>

static const qreal kBorderWidth = 2;

namespace ive {

AADLFunctionTypeGraphicsItem::AADLFunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parent)
    : AADLRectGraphicsItem(entity, parent)
    , m_textItem(new AADLFunctionNameGraphicsItem(this))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ZOrder.Function);
}

ivm::IVFunctionType *AADLFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVFunctionType *>(m_dataObject);
}

void AADLFunctionTypeGraphicsItem::init()
{
    AADLRectGraphicsItem::init();
    m_textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_textItem->setFont(font());
    updateText();

    connect(m_textItem, &shared::ui::TextItem::edited, this, &AADLFunctionTypeGraphicsItem::updateNameFromUi);
    connect(entity(), qOverload<ivm::meta::Props::Token>(&ivm::IVFunction::attributeChanged), this,
            [this](ivm::meta::Props::Token attr) {
                if (attr == ivm::meta::Props::Token::name || attr == ivm::meta::Props::Token::url) {
                    updateText();
                }
            });
    connect(entity(), &ivm::IVFunction::titleChanged, this, &AADLFunctionTypeGraphicsItem::updateText);
    connect(m_textItem, &AADLFunctionNameGraphicsItem::textChanged, this, [this]() { updateTextPosition(); });
}

void AADLFunctionTypeGraphicsItem::rebuildLayout()
{
    AADLRectGraphicsItem::rebuildLayout();
    updateTextPosition();
    for (auto child : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(child))
            iface->instantLayoutUpdate();
    }
}

void AADLFunctionTypeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

QSizeF AADLFunctionTypeGraphicsItem::minimalSize() const
{
    const QSizeF textSize = m_textItem->boundingRect().size();
    return { qMax(textSize.width(), DefaultGraphicsItemSize.width()),
        qMax(textSize.height(), DefaultGraphicsItemSize.height()) };
}

void AADLFunctionTypeGraphicsItem::updateTextPosition()
{
    m_textItem->adjustSize();

    QRectF textRect = m_textItem->boundingRect();
    const QRectF targetTextRect = boundingRect().marginsRemoved(kTextMargins);

    const QSizeF maxTxtSize = targetTextRect.size();
    const QSizeF txtSize = textRect.size();
    if (txtSize.width() > maxTxtSize.width() || txtSize.height() > maxTxtSize.height()) {
        m_textItem->setExplicitSize(maxTxtSize);
        textRect = m_textItem->boundingRect();
    }

    prepareTextRect(textRect, targetTextRect);
    m_textItem->setPos(textRect.topLeft());
}

shared::ColorManager::HandledColors AADLFunctionTypeGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::FunctionType;
}

void AADLFunctionTypeGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    setPen(h.pen());
    setBrush(h.brush());
    update();
}

void AADLFunctionTypeGraphicsItem::updateNameFromUi(const QString &name)
{
    if (name == entity()->titleUI()) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for AADLFunctionTypeGraphicsItem";
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

    const QVariantHash attributess = { { ivm::meta::Props::token(ivm::meta::Props::Token::name), newName } };
    const auto attributesCmd = new cmd::CmdEntityAttributeChange(entity(), attributess);
    m_commandsStack->push(attributesCmd);
}

void AADLFunctionTypeGraphicsItem::updateText()
{
    const QString text = entity()->titleUI();
    static const QString urlAttrName { ivm::meta::Props::token(ivm::meta::Props::Token::url) };
    if (m_dataObject->hasAttribute(urlAttrName)) {
        const QString url = m_dataObject->attr(urlAttrName).toString();
        const QString html = QStringLiteral("<a href=\"%1\">%2</a>").arg(url, text);
        if (html != m_textItem->toHtml()) {
            m_textItem->setHtml(html);
            updateTextPosition();
        }
    } else if (text != m_textItem->toPlainText()) {
        m_textItem->setPlainText(entity()->titleUI());
        updateTextPosition();
    }
}

void AADLFunctionTypeGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QRectF rect = sceneBoundingRect();

    AADLRectGraphicsItem::onManualResizeProgress(grip, from, to);
    const QPointF delta = to - from;
    if (delta.isNull()) {
        return;
    }
    for (auto child : childItems()) {
        if (auto iface = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(child)) {
            const ivm::IVInterface *obj = iface->entity();
            Q_ASSERT(obj);
            if (!obj) {
                return;
            }

            const QPointF storedPos = ive::pos(obj->coordinates());
            if (storedPos.isNull() || !grip) {
                iface->instantLayoutUpdate();
                continue;
            }

            const Qt::Alignment side = getNearestSide(rect, storedPos);
            const QRectF sceneRect = sceneBoundingRect();
            const QPointF pos = getSidePosition(sceneRect, storedPos, side);
            iface->setPos(iface->parentItem()->mapFromScene(pos));
        }
    }
}

QString AADLFunctionTypeGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<ivm::IVFunctionType *>({ entity() }, QString());
    const QString instances = uniteNames<QPointer<ivm::IVFunction>>(entity()->instances(), tr("Instances: "));
    const QString ris = uniteNames<ivm::IVInterface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<ivm::IVInterface *>(entity()->pis(), tr("PI: "));

    return joinNonEmpty({ title, instances, ris, pis }, QStringLiteral("<br>"));
}

bool AADLFunctionTypeGraphicsItem::isRootItem() const
{
    return !parentItem() && entity() && entity()->isRootObject();
}

void AADLFunctionTypeGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveTopLeft(targetTextRect.topLeft());
}

}
