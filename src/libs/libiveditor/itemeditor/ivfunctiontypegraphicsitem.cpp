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
#include "commandsstack.h"
#include "common.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "ivcommonprops.h"
#include "ivconnection.h"
#include "ivconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivfunctionnamegraphicsitem.h"
#include "ivinterfacegraphicsitem.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "parameter.h"
#include "ui/textitem.h"
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

QSizeF IVFunctionTypeGraphicsItem::minimalSize() const
{
    const QSizeF textSize = m_textItem->boundingRect().size();
    return { qMax(textSize.width(), shared::graphicsviewutils::kDefaultGraphicsItemSize.width()),
        qMax(textSize.height(), shared::graphicsviewutils::kDefaultGraphicsItemSize.height()) };
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
    if (!m_textItem) {
        return;
    }

    const QRectF targetTextRect = boundingRect().marginsRemoved(shared::graphicsviewutils::kTextMargins);

    m_textItem->setExplicitSize(QSizeF());
    m_textItem->setTextWidth(-1);

    const QSizeF maxTxtSize = targetTextRect.size();
    const QSizeF txtSize = m_textItem->document()->size();
    if (txtSize.width() > maxTxtSize.width() || txtSize.height() > maxTxtSize.height()) {
        m_textItem->setExplicitSize(maxTxtSize);
    }

    QRectF textRect = m_textItem->boundingRect();
    prepareTextRect(textRect, targetTextRect);
    m_textItem->setPos(textRect.topLeft());
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

bool IVFunctionTypeGraphicsItem::isRootItem() const
{
    return !parentItem() && entity() && entity()->isRootObject();
}

void IVFunctionTypeGraphicsItem::prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const
{
    textRect.moveTopLeft(targetTextRect.topLeft());
}

shared::ui::TextItem *IVFunctionTypeGraphicsItem::initTextItem()
{
    shared::ui::TextItem *textItem = new IVFunctionNameGraphicsItem(this);
    connect(textItem, &shared::ui::TextItem::edited, this, &IVFunctionTypeGraphicsItem::updateNameFromUi);
    connect(entity(), &ivm::IVFunction::attributeChanged, this, [this](const QString &token) {
        const ivm::meta::Props::Token attr = ivm::meta::Props::token(token);
        if (attr == ivm::meta::Props::Token::name || attr == ivm::meta::Props::Token::url) {
            updateText();
        }
    });
    textItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
    textItem->setFont(font());
    return textItem;
}

}
