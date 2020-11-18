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
#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctionnamegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/textgraphicsitem.h"
#include "colors/colormanager.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPainter>
#include <QtDebug>

static const qreal kBorderWidth = 2;

namespace aadlinterface {

AADLFunctionTypeGraphicsItem::AADLFunctionTypeGraphicsItem(aadl::AADLObjectFunctionType *entity, QGraphicsItem *parent)
    : AADLRectGraphicsItem(entity, parent)
    , m_textItem(new AADLFunctionNameGraphicsItem(this))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ZOrder.Function);
}

aadl::AADLObjectFunctionType *AADLFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectFunctionType *>(aadlObject());
}

void AADLFunctionTypeGraphicsItem::init()
{
    AADLRectGraphicsItem::init();
    m_textItem->setPlainText(entity()->titleUI());
    m_textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_textItem->setFont(font());

    connect(m_textItem, &TextGraphicsItem::edited, this, &AADLFunctionTypeGraphicsItem::updateNameFromUi);
    connect(entity(), qOverload<aadl::meta::Props::Token>(&aadl::AADLObjectFunction::attributeChanged), this,
            [this](aadl::meta::Props::Token attr) {
                if (attr == aadl::meta::Props::Token::name) {
                    const QString txt = entity()->titleUI();
                    if (m_textItem->toPlainText() != txt) {
                        m_textItem->setPlainText(txt);
                        updateTextPosition();
                    }
                }
            });
    connect(entity(), &aadl::AADLObjectFunction::titleChanged, this, [this](const QString &text) {
        m_textItem->setPlainText(text);
        instantLayoutUpdate();
    });
    connect(m_textItem, &AADLFunctionNameGraphicsItem::editingModeOff, this, [this]() { updateTextPosition(); });
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
    painter->drawRect(
            boundingRect().adjusted(kBorderWidth / 2, kBorderWidth / 2, -kBorderWidth / 2, -kBorderWidth / 2));
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

ColorManager::HandledColors AADLFunctionTypeGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::FunctionType;
}

void AADLFunctionTypeGraphicsItem::applyColorScheme()
{
    const ColorHandler &h = colorHandler();
    setPen(h.pen());
    setBrush(h.brush());
    update();
}

void AADLFunctionTypeGraphicsItem::updateNameFromUi(const QString &name)
{
    if (name == entity()->titleUI()) {
        return;
    }
    if (!aadl::AADLNameValidator::isAcceptableName(entity(), name)) {
        m_textItem->setPlainText(entity()->titleUI());
        return;
    }

    const QString newName = aadl::AADLNameValidator::encodeName(aadlObject()->aadlType(), name);
    const QVariantMap attributess = { { aadl::meta::Props::token(aadl::meta::Props::Token::name), newName } };
    if (const auto attributesCmd = cmd::CommandsFactory::create(
                cmd::ChangeEntityAttributes, { QVariant::fromValue(entity()), QVariant::fromValue(attributess) })) {
        cmd::CommandsStack::push(attributesCmd);
    }
}

QString AADLFunctionTypeGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<aadl::AADLObjectFunctionType *>({ entity() }, QString());
    const QString instances = uniteNames<QPointer<aadl::AADLObjectFunction>>(entity()->instances(), tr("Instances: "));
    const QString ris = uniteNames<aadl::AADLObjectIface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<aadl::AADLObjectIface *>(entity()->pis(), tr("PI: "));

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
