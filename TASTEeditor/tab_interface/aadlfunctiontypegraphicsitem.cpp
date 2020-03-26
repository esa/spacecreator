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
#include "tab_aadl/aadlnamevalidator.h"
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
    : AADLRectGraphicsItem(entity, parent)
    , m_textItem(new AADLFunctionNameGraphicsItem(this))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(utils::kFunctionZLevel);

    if (entity) {
        m_textItem->setPlainText(entity->title());
        m_textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);

        connect(m_textItem, &TextGraphicsItem::edited, this, [this, entity](const QString &text) {
            if (!AADLNameValidator::isAcceptableName(entity, text)) {
                m_textItem->setPlainText(entity->title());
                return;
            }

            const QVariantMap attributess = { { meta::Props::token(meta::Props::Token::name), text } };
            if (const auto attributesCmd = cmd::CommandsFactory::create(
                        cmd::ChangeEntityAttributes, { QVariant::fromValue(entity), QVariant::fromValue(attributess) }))
                taste3::cmd::CommandsStack::current()->push(attributesCmd);
        });
        connect(entity, &AADLObjectFunction::attributeChanged, this,
                [this, entity](taste3::aadl::meta::Props::Token attr) {
                    if (attr == taste3::aadl::meta::Props::Token::name) {
                        const QString txt = entity->title();
                        if (m_textItem->toPlainText() != txt) {
                            m_textItem->setPlainText(txt);
                            updateTextPosition();
                        }
                    }
                });
        connect(entity, &AADLObjectFunction::titleChanged, this, [this](const QString &text) {
            m_textItem->setPlainText(text);
            instantLayoutUpdate();
        });
        connect(m_textItem, &AADLFunctionNameGraphicsItem::editingModeOff, this, [this]() { updateTextPosition(); });
    }
    applyColorScheme();
}

AADLObjectFunctionType *AADLFunctionTypeGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectFunctionType *>(aadlObject());
}

void AADLFunctionTypeGraphicsItem::rebuildLayout()
{
    AADLRectGraphicsItem::rebuildLayout();
    updateTextPosition();
    for (auto child : childItems()) {
        if (auto iface = qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(child))
            iface->instantLayoutUpdate();
    }
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

QSizeF AADLFunctionTypeGraphicsItem::minimalSize() const
{
    const QSizeF textSize = m_textItem->boundingRect().size();
    return { qMax(textSize.width(), utils::DefaultGraphicsItemSize.width()),
             qMax(textSize.height(), utils::DefaultGraphicsItemSize.height()) };
}

void AADLFunctionTypeGraphicsItem::updateTextPosition()
{
    m_textItem->adjustSize();

    QRectF textRect = m_textItem->boundingRect();
    const QRectF targetTextRect = boundingRect().marginsRemoved(utils::kTextMargins);

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

QString AADLFunctionTypeGraphicsItem::prepareTooltip() const
{
    const QString title = uniteNames<AADLObjectFunctionType *>({ entity() }, QString());
    const QString instances = uniteNames<QPointer<AADLObjectFunction>>(entity()->instances(), tr("Instances: "));
    const QString ris = uniteNames<AADLObjectIface *>(entity()->ris(), tr("RI: "));
    const QString pis = uniteNames<AADLObjectIface *>(entity()->pis(), tr("PI: "));

    return joinNonEmpty({ title, instances, ris, pis }, QStringLiteral("<br>"));
}

void AADLFunctionTypeGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveTopLeft(targetTextRect.topLeft());
}

} // namespace aadl
} // namespace taste3
