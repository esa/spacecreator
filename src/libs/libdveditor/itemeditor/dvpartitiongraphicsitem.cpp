/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvpartitiongraphicsitem.h"

#include "commands/cmdentityattributechange.h"
#include "commandsstackbase.h"
#include "dvnamevalidator.h"
#include "graphicsviewutils.h"
#include "ui/textitem.h"

#include <QDebug>
#include <QPainter>

namespace dve {

DVPartitionGraphicsItem::DVPartitionGraphicsItem(dvm::DVPartition *partition, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(partition, parent)
{
}

void DVPartitionGraphicsItem::init()
{
    shared::ui::VERectGraphicsItem::init();
    connect(entity(), &dvm::DVPartition::functionAdded, this, [this]() { update(); });
    connect(entity(), &dvm::DVPartition::functionRemoved, this, [this]() { update(); });
    connect(entity(), &dvm::DVPartition::functionChanged, this, [this]() { update(); });
}

dvm::DVPartition *DVPartitionGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVPartition *>();
}

QSizeF DVPartitionGraphicsItem::minimalSize() const
{
    return QSizeF(80, 80);
}

int DVPartitionGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

void DVPartitionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVPartitionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!entity())
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    QRectF br = boundingRect();
    painter->drawRect(br);
    painter->setFont(font());

    const QString functions = entity()->functionsNames().join(QLatin1Char('\n'));
    painter->setPen(QPen(Qt::gray));
    QFont f(font());
    f.setItalic(true);
    if (m_textItem) {
        br.setTop(mapFromItem(m_textItem, m_textItem->boundingRect().bottomLeft()).y());
    }
    painter->drawText(br, Qt::AlignCenter, functions);

    painter->restore();
}

shared::ColorManager::HandledColors DVPartitionGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Partition;
}

shared::ui::TextItem *DVPartitionGraphicsItem::initTextItem()
{
    auto textItem = shared::ui::VERectGraphicsItem::initTextItem();
    connect(entity(), &dvm::DVObject::titleChanged, this, &DVPartitionGraphicsItem::updateText);
    connect(entity(), &dvm::DVObject::urlChanged, this, &DVPartitionGraphicsItem::updateText);
    connect(textItem, &shared::ui::TextItem::edited, this, &DVPartitionGraphicsItem::updateEntityTitle);
    textItem->setHtml(entity()->titleUI());
    return textItem;
}

void DVPartitionGraphicsItem::updateTextPosition()
{
    if (m_textItem) {
        const QRectF targetTextRect = boundingRect();
        m_textItem->setTextWidth(targetTextRect.width());
        m_textItem->setPos(targetTextRect.topLeft());
    }
}

void DVPartitionGraphicsItem::updateEntityTitle(const QString &text)
{
    const QString newName = dvm::DVNameValidator::encodeName(entity()->type(), text);
    if (newName == entity()->title()) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for DVPartitionGraphicsItem";
        return;
    }

    if (!dvm::DVNameValidator::isAcceptableName(entity(), newName)) {
        return;
    }
    const QVariantHash attributes = { { dvm::meta::Props::token(dvm::meta::Props::Token::name), newName } };
    m_commandsStack->push(new shared::cmd::CmdEntityAttributeChange(entity(), attributes));
}

} // namespace dve
