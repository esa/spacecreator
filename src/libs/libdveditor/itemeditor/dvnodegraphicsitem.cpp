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

#include "dvnodegraphicsitem.h"

#include "commands/cmdentityattributeschange.h"
#include "commandsstackbase.h"
#include "dvconnectiongraphicsitem.h"
#include "dvdevicegraphicsitem.h"
#include "dvnamevalidator.h"
#include "dvpropertytemplateconfig.h"
#include "graphicsviewutils.h"
#include "ui/textitem.h"

#include <QPainter>
#include <QTextDocument>
#include <QtDebug>

namespace dve {

DVNodeGraphicsItem::DVNodeGraphicsItem(dvm::DVNode *entity, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(entity, parent)
{
}

dvm::DVNode *dve::DVNodeGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVNode *>();
}

void DVNodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!entity())
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    const QRectF br = boundingRect();
    painter->drawRoundedRect(br, 5, 5);
    painter->drawRoundedRect(br.adjusted(2, 2, -2, -2), 4, 4);
    painter->restore();
}

QSizeF DVNodeGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultNodeGraphicsItemSize;
}

int DVNodeGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

void DVNodeGraphicsItem::childBoundingBoxChanged()
{
    rebuildLayout();
    layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Rebuild);
}

shared::ColorManager::HandledColors DVNodeGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Node;
}

void DVNodeGraphicsItem::rebuildLayout()
{
    shared::ui::VERectGraphicsItem::rebuildLayout();
    const QRectF nestedContent = nestedItemsSceneBoundingRect();
    if (nestedContent.isValid()) {
        const QRectF contentRect = nestedContent.marginsAdded(shared::graphicsviewutils::kContentMargins);
        if (!sceneBoundingRect().contains(contentRect)) {
            const QRectF upcomingRect = sceneBoundingRect() | contentRect;
            if (!shared::graphicsviewutils::isCollided(this, upcomingRect)) {
                setRect(upcomingRect);
                mergeGeometry();
            }
        }
    }
}

void DVNodeGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

shared::ui::TextItem *DVNodeGraphicsItem::initTextItem()
{
    auto textItem = shared::ui::VERectGraphicsItem::initTextItem();
    connect(entity(), &dvm::DVObject::titleChanged, this, &DVNodeGraphicsItem::updateText);
    connect(entity(), &dvm::DVObject::urlChanged, this, &DVNodeGraphicsItem::updateText);
    connect(textItem, &shared::ui::TextItem::edited, this, &DVNodeGraphicsItem::updateEntityTitle);
    textItem->setHtml(entity()->titleUI());
    return textItem;
}

void DVNodeGraphicsItem::updateEntityTitle(const QString &text)
{
    const QString newName = dvm::DVNameValidator::encodeName(entity()->type(), text);
    if (newName == entity()->title()) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for DVNodeGraphicsItem";
        return;
    }

    if (!dvm::DVNameValidator::isAcceptableName(entity(), newName)) {
        return;
    }
    const QList<EntityAttribute> attributes = { EntityAttribute {
            dvm::meta::Props::token(dvm::meta::Props::Token::name), newName, EntityAttribute::Type::Attribute } };
    m_commandsStack->push(new shared::cmd::CmdEntityAttributesChange(
            dvm::DVPropertyTemplateConfig::instance(), entity(), attributes));
}

} // namespace dve
