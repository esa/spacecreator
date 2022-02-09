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

#include "ivcommentgraphicsitem.h"

#include "colors/colormanager.h"
#include "commands/cmdentityattributeschange.h"
#include "commandsstackbase.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "ivcomment.h"
#include "ivcoreutils.h"
#include "ivfunctiongraphicsitem.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"
#include "ui/textitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include <QtDebug>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace ive {

IVCommentGraphicsItem::IVCommentGraphicsItem(ivm::IVComment *comment, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(comment, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFont(qApp->font());
    setZValue(ZOrder.Comment);
}

void IVCommentGraphicsItem::init()
{
    shared::ui::VERectGraphicsItem::init();
    updateText();
}

int IVCommentGraphicsItem::itemLevel(bool isSelected) const
{
    return gi::itemLevel(entity(), isSelected);
}

ivm::IVComment *IVCommentGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVComment *>(m_dataObject);
}

void IVCommentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());

    const QRectF br = boundingRect();
    auto preparePolygon = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() - QPointF(kMargins, 0), rect.topLeft(), rect.bottomLeft(),
            rect.bottomRight(), rect.topRight() + QPointF(0, kMargins), rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolygon(preparePolygon(br));

    auto preparePolyline = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() + QPointF(0, kMargins),
            rect.topRight() - QPointF(kMargins, -kMargins), rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolyline(preparePolyline(br));
    painter->restore();
    shared::ui::VERectGraphicsItem::paint(painter, option, widget);
}

void IVCommentGraphicsItem::rebuildLayout()
{
    shared::ui::VERectGraphicsItem::rebuildLayout();
    setVisible(entity()
            && (ivm::utils::nestingLevel(entity()) >= gi::kNestingVisibilityLevel || entity()->isRootObject())
            && entity()->isVisible());
    update();
}

QSizeF IVCommentGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultGraphicsItemSize;
}

shared::ColorManager::HandledColors IVCommentGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Comment;
}

shared::ui::TextItem *IVCommentGraphicsItem::initTextItem()
{
    auto textItem = new shared::ui::TextItem(this);
    connect(entity(), &ivm::IVObject::titleChanged, this, &IVCommentGraphicsItem::updateText);
    connect(textItem, &shared::ui::TextItem::edited, this, &IVCommentGraphicsItem::updateEntityTitle);
    textItem->setTextMargin(kMargins);
    textItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
    textItem->setFont(font());
    textItem->setTextWrapMode(QTextOption::WordWrap);
    textItem->setFlags(QGraphicsItem::ItemClipsToShape);
    textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    textItem->setBackground(Qt::transparent);
    textItem->setOpenExternalLinks(true);
    return textItem;
}

void IVCommentGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void IVCommentGraphicsItem::updateEntityTitle(const QString &text)
{
    const QString newName = ivm::IVNameValidator::encodeName(entity()->type(), text);
    if (newName == entity()->title()) {
        return;
    }
    if (!ivm::IVNameValidator::isAcceptableName(entity(), newName)) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for IVCommentGraphicsItem";
        return;
    }

    const QList<EntityAttribute> attributes = { EntityAttribute {
            ivm::meta::Props::token(ivm::meta::Props::Token::name), newName, EntityAttribute::Type::Attribute } };
    m_commandsStack->push(new shared::cmd::CmdEntityAttributesChange(
            ivm::IVPropertyTemplateConfig::instance(), entity(), attributes));
}

}
