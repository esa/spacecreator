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
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "ivinterfacegraphicsitem.h"

#include "colors/colormanager.h"
#include "commands/cmdentityattributeschange.h"
#include "commandsstackbase.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "ivcommentgraphicsitem.h"
#include "ivconnection.h"
#include "ivconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivfunctiontypegraphicsitem.h"
#include "ivinterface.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"
#include "positionlookuphelper.h"
#include "ui/textitem.h"
#include "ui/veconnectiongraphicsitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

static const qreal kHeight = shared::graphicsviewutils::kInterfaceBaseLength * 4 / 5;
static const QColor kSelectedBackgroundColor = QColor(Qt::magenta);
static const QColor kDefaultBackgroundColor = QColor(Qt::blue);
static const int kTextMargin = 2;

namespace ive {

IVInterfaceGraphicsItem::IVInterfaceGraphicsItem(ivm::IVInterface *entity, QGraphicsItem *parent)
    : shared::ui::VEConnectionEndPointGraphicsItem(entity, parent)
    , m_type(new QGraphicsPathItem(this))
    , m_iface(new QGraphicsPathItem(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ZOrder.Interface);
}

ivm::IVInterface *IVInterfaceGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVInterface *>(m_dataObject);
}

void IVInterfaceGraphicsItem::init()
{
    shared::ui::VEInteractiveObject::init();

    connect(entity(), &ivm::IVObject::attributeChanged, this, &IVInterfaceGraphicsItem::onAttrOrPropChanged);
    if (auto ri = qobject_cast<ivm::IVInterfaceRequired *>(entity()))
        connect(ri, &ivm::IVInterfaceRequired::inheritedLabelsChanged, this, &IVInterfaceGraphicsItem::updateLabel);
    connect(this, &IVInterfaceGraphicsItem::needUpdateLayout, this, [this]() {
        if (auto function = targetItem()) {
            function->update();
        }
    });

    updateIface();
    updateKind();
    updateLabel();
    setInterfaceName(ifaceLabel());
}

void IVInterfaceGraphicsItem::setInterfaceName(const QString &name)
{
    QString text = ivm::IVNameValidator::decodeName(entity()->type(), name);

    int maxTextWidth = maxWidth();
    if (maxTextWidth > 0) {
        // text is always at 0, interface item might (probably is) negative
        maxTextWidth = maxTextWidth + (boundingRect().x() - kTextMargin);
        const QFontMetrics fm(m_textItem->font());
        text = fm.elidedText(text, Qt::ElideRight, maxTextWidth);
    }

    if (text != m_textItem->toPlainText()) {
        m_textItem->setPlainText(text);
        instantLayoutUpdate();
    }
}

QPointF IVInterfaceGraphicsItem::connectionEndPoint(const bool nestedConnection) const
{
    const qreal borderWidth = m_iface->pen().widthF() + 1;
    const QRectF ifaceRect =
            m_iface->sceneBoundingRect().adjusted(borderWidth / 2, borderWidth / 2, -borderWidth / 2, -borderWidth / 2);
    if (!ifaceRect.isValid()) {
        return {};
    }
    if (auto parentGraphicsItem = parentItem()) {
        const QRectF parentRect = parentGraphicsItem->boundingRect();
        const Qt::Alignment alignment = shared::graphicsviewutils::getNearestSide(parentRect, pos());
        switch (alignment) {
        case Qt::AlignLeft:
            if (nestedConnection) {
                return { ifaceRect.right(), QLineF(ifaceRect.topRight(), ifaceRect.bottomRight()).center().y() };
            } else {
                return { ifaceRect.left(), QLineF(ifaceRect.topLeft(), ifaceRect.bottomLeft()).center().y() };
            }
        case Qt::AlignTop:
            if (nestedConnection) {
                return { QLineF(ifaceRect.bottomLeft(), ifaceRect.bottomRight()).center().x(), ifaceRect.bottom() };
            } else {
                return { QLineF(ifaceRect.topLeft(), ifaceRect.topRight()).center().x(), ifaceRect.top() };
            }
        case Qt::AlignRight:
            if (nestedConnection) {
                return { ifaceRect.left(), QLineF(ifaceRect.topLeft(), ifaceRect.bottomLeft()).center().y() };
            } else {
                return { ifaceRect.right(), QLineF(ifaceRect.topRight(), ifaceRect.bottomRight()).center().y() };
            }
        case Qt::AlignBottom:
            if (nestedConnection) {
                return { QLineF(ifaceRect.topLeft(), ifaceRect.topRight()).center().x(), ifaceRect.top() };
            } else {
                return { QLineF(ifaceRect.bottomLeft(), ifaceRect.bottomRight()).center().x(), ifaceRect.bottom() };
            }
        default:
            return {};
        }
    }
    return {};
}

QPointF IVInterfaceGraphicsItem::connectionEndPoint(shared::ui::VEConnectionGraphicsItem *connection) const
{
    if (connection) {
        const bool innerConnection = connection->entity()->parentObject() == entity()->parentObject();
        return connectionEndPoint(innerConnection);
    }
    return m_iface->sceneBoundingRect().center();
}

QPainterPath IVInterfaceGraphicsItem::ifaceShape() const
{
    const QRectF parentRect = parentItem()->boundingRect();
    const Qt::Alignment alignment = shared::graphicsviewutils::getNearestSide(parentRect, pos());
    return mapToScene(ifaceTransform(alignment).map(ifacePath()));
}

void IVInterfaceGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    m_iface->setTransform(ifaceTransform(alignment));
    m_type->setTransform(typeTransform(alignment));
    updateText();

    m_shape = composeShape();
    setBoundingRect(shape().boundingRect());
}

int IVInterfaceGraphicsItem::itemLevel(bool isSelected) const
{
    return gi::itemLevel(entity(), isSelected);
}

QPainterPath IVInterfaceGraphicsItem::shape() const
{
    if (m_shape.isEmpty())
        return composeShape();

    return m_shape;
}

void IVInterfaceGraphicsItem::updateFromEntity()
{
    setInterfaceName(ifaceLabel());
    shared::ui::VEConnectionEndPointGraphicsItem::updateFromEntity();
}

void IVInterfaceGraphicsItem::onSelectionChanged(bool isSelected)
{
    const shared::ColorHandler &h = colorHandler();
    m_iface->setBrush(isSelected ? kSelectedBackgroundColor : h.brush());
}

/*!
   Returns the maximal width of this item. The width is limited by other functions and interfaces on the right.
   If there is no other item on the right, -1 (unlimited) is returned.
 */
qreal IVInterfaceGraphicsItem::maxWidth() const
{
    if (!scene()) {
        return -1.;
    }

    const QRectF itemRect = sceneBoundingRect();
    const QRectF sceneRect = scene()->sceneRect();
    const QRectF parentRect = targetItem()->boundingRect();
    const QPointF ifacePos = pos();
    const Qt::Alignment side = shared::graphicsviewutils::getNearestSide(parentRect, ifacePos);
    QRectF rect;
    rect.setTop(itemRect.top());
    rect.setBottom(itemRect.bottom());
    if (side == Qt::AlignLeft) {
        rect.setLeft(sceneRect.left());
        rect.setRight(scenePos().x());
    } else {
        rect.setLeft(scenePos().x());
        rect.setRight(sceneRect.right());
    }

    for (QGraphicsItem *rootItem : scene()->items(rect)) {
        if (auto rectItem = qobject_cast<shared::ui::VERectGraphicsItem *>(rootItem->toGraphicsObject())) {
            QList<QGraphicsItem *> items;
            for (QGraphicsItem *item : rootItem->childItems()) {
                if (item->type() == ive::IVFunctionTypeGraphicsItem::Type
                        || item->type() == ive::IVInterfaceGraphicsItem::Type) {
                    items.append(item);
                }
            }
            items.append(rootItem);

            for (QGraphicsItem *item : items) {
                if (item->isVisible() && item != this) {
                    const QRectF otherRect = item->sceneBoundingRect();
                    if (otherRect.bottom() > itemRect.top() && otherRect.top() < itemRect.bottom()) {
                        // the items do vertically intersect

                        if (otherRect.contains(scenePos()))
                            continue;

                        if (side == Qt::AlignLeft) {
                            if (otherRect.right() < rect.right() && otherRect.right() > rect.left()) {
                                rect.setLeft(otherRect.right());
                            }
                        } else {
                            if (otherRect.left() > rect.left() && otherRect.left() < rect.right()) {
                                rect.setRight(otherRect.left());
                            }
                        }
                    }
                }
            }
        }
    }

    return rect.width();
}

shared::ColorManager::HandledColors IVInterfaceGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Iface;
}

shared::ui::TextItem *IVInterfaceGraphicsItem::initTextItem()
{
    auto textItem = new shared::ui::TextItem(this);
    connect(entity(), &ivm::IVInterface::titleChanged, this, &IVInterfaceGraphicsItem::updateLabel);
    connect(textItem, &shared::ui::TextItem::edited, this, &IVInterfaceGraphicsItem::updateEntityTitle);
    textItem->setEditable(false);
    textItem->setFont(font());
    textItem->setBackground(Qt::transparent);
    textItem->setTextWrapMode(QTextOption::NoWrap);
    textItem->setTextInteractionFlags(Qt::NoTextInteraction);
    textItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
    textItem->setHtml(entity()->titleUI());
    return textItem;
}

void IVInterfaceGraphicsItem::updateTextPosition()
{
    if (m_textItem) {
        const QRectF parentRect = targetItem()->boundingRect();
        const QPointF ifacePos = pos();
        const Qt::Alignment side = shared::graphicsviewutils::getNearestSide(parentRect, ifacePos);
        m_textItem->setPos(m_textItem->mapToParent(textTransform(side).map(QPointF(0, 0))));
    }
}

qreal IVInterfaceGraphicsItem::typeIconHeight() const
{
    return kHeight;
}

void IVInterfaceGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    m_type->setPen(pen);
    pen.setWidthF(pen.widthF() / 2.);
    m_iface->setPen(pen);
    m_iface->setBrush(h.brush());
    update();
}

void IVInterfaceGraphicsItem::updateLabel()
{
    setInterfaceName(ifaceLabel());
    m_shape = composeShape();
}

QVariant IVInterfaceGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemParentChange) {
        if (auto currentParentObj = parentObject()) {
            disconnect(this, &VEInteractiveObject::relocated, currentParentObj, nullptr);
        }
    } else if (change == QGraphicsItem::ItemParentHasChanged) {
        if (auto currentParentObj = parentObject()) {
            connect(this, &VEInteractiveObject::relocated, currentParentObj,
                    [currentParentObj]() { currentParentObj->update(); });
        }
    }
    return shared::ui::VEConnectionEndPointGraphicsItem::itemChange(change, value);
}

void IVInterfaceGraphicsItem::updateKind()
{
    m_type->setPath(typePath());
    m_shape = composeShape();
}

void IVInterfaceGraphicsItem::updateIface()
{
    m_iface->setPath(ifacePath());
    m_shape = composeShape();
}

QString IVInterfaceGraphicsItem::ifaceLabel() const
{
    if (entity()) {
        return entity()->ifaceLabel();
    }
    return {};
}

QString IVInterfaceGraphicsItem::prepareTooltip() const
{
    QString toolTip = shared::ui::VEInteractiveObject::prepareTooltip();
    if (entity()->isProvided()) {
        toolTip = shared::joinNonEmpty(
                { toolTip, shared::uniteNames(entity()->params(), tr("Parameters:")) }, QStringLiteral("<br>"));
        return toolTip;
    }

    auto ri = entity()->as<const ivm::IVInterfaceRequired *>();
    if (!ri)
        return toolTip;

    const QString label = ivm::IVNameValidator::decodeName(entity()->type(), ifaceLabel());
    if (toolTip != label)
        toolTip = QString("%1<br><i><b>%2</b></i>").arg(label, toolTip);

    toolTip = shared::joinNonEmpty(
            { toolTip, shared::uniteNames(entity()->params(), tr("Parameters:")) }, QStringLiteral("<br>"));
    return toolTip;
}

void IVInterfaceGraphicsItem::onAttrOrPropChanged(const QString &attrName)
{
    const ivm::meta::Props::Token t = ivm::meta::Props::token(attrName);
    switch (t) {
        //    ivm::meta::Props::Token::name: // handled in IVInterfaceGraphicsItem::updateLabel
    case ivm::meta::Props::Token::InheritPI: {
#ifdef QT_DEBUG
        qWarning() << "check for a call duplication" << t;
#endif
        updateLabel();
        break;
    }
    case ivm::meta::Props::Token::kind:
        updateKind();
        break;
    default:
        break;
    }
}

QTransform IVInterfaceGraphicsItem::typeTransform(Qt::Alignment alignment) const
{
    const qreal offset = shared::graphicsviewutils::kInterfaceBaseLength + 2;

    QPointF shift(0., 0.);
    switch (alignment) {
    case Qt::AlignLeft:
        shift.setX(-offset);
        break;
    case Qt::AlignRight:
        shift.setX(offset);
        break;
    case Qt::AlignTop:
        shift.setY(-offset);
        break;
    case Qt::AlignBottom:
        shift.setY(offset);
        break;
    }

    return QTransform().translate(shift.x(), shift.y());
}

QTransform IVInterfaceGraphicsItem::ifaceTransform(Qt::Alignment alignment) const
{
    const bool insideOut = entity()->direction() == ivm::IVInterface::InterfaceType::Required;
    qreal rotationDegree = 0.;
    switch (alignment) {
    case Qt::AlignLeft:
        rotationDegree = insideOut ? 180 : 0;
        break;
    case Qt::AlignRight:
        rotationDegree = insideOut ? 0 : 180;
        break;
    case Qt::AlignTop:
        rotationDegree = insideOut ? 270 : 90;
        break;
    case Qt::AlignBottom:
        rotationDegree = insideOut ? 90 : 270;
        break;
    }

    return QTransform().rotate(rotationDegree);
}

QTransform IVInterfaceGraphicsItem::textTransform(Qt::Alignment alignment) const
{
    static const qreal kOffset = 2;
    QRectF textRect = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    const QRectF ifaceRect = mapRectFromItem(m_type, m_type->boundingRect());
    switch (alignment) {
    case Qt::AlignLeft:
        textRect.moveBottomRight(QPointF(-kOffset, ifaceRect.top() + kOffset));
        break;
    case Qt::AlignRight:
        textRect.moveBottomLeft(QPointF(kOffset, ifaceRect.top() + kOffset));
        break;
    case Qt::AlignTop:
        textRect.moveBottomLeft(QPointF(ifaceRect.right() - kOffset, -kOffset));
        break;
    case Qt::AlignBottom:
        textRect.moveTopLeft(QPointF(ifaceRect.right() - kOffset, kOffset));
        break;
    default:
        return {};
    }
    const QPointF offset = textRect.topLeft() - m_textItem->pos();
    return QTransform().translate(offset.x(), offset.y());
}

QPainterPath IVInterfaceGraphicsItem::ifacePath() const
{
    static const qreal kBase = shared::graphicsviewutils::kInterfaceBaseLength;

    QPainterPath path;
    path.addPolygon(QVector<QPointF> {
            QPointF(-kHeight / 3, -kBase / 2), QPointF(-kHeight / 3, kBase / 2), QPointF(2 * kHeight / 3, 0) });
    path.closeSubpath();
    return path;
}

QPainterPath IVInterfaceGraphicsItem::typePath() const
{
    auto iface = qobject_cast<ivm::IVInterface *>(entity());
    if (!iface)
        return {};

    QPainterPath kindPath;
    switch (iface->kind()) {
    case ivm::IVInterface::OperationKind::Cyclic: {
        const qreal kindBaseValue = kHeight;
        kindPath.arcTo({ kindPath.currentPosition().x() - kindBaseValue / 2,
                               kindPath.currentPosition().y() - kindBaseValue, kindBaseValue, kindBaseValue },
                -90, -270);
        kindPath.lineTo(kindPath.currentPosition() + QPointF(0, kindBaseValue / 3));
        kindPath.addPolygon(
                QVector<QPointF> { kindPath.currentPosition() + QPointF(-kindBaseValue / 3, -kindBaseValue / 3),
                        kindPath.currentPosition(),
                        kindPath.currentPosition() + QPointF(kindBaseValue / 3, -kindBaseValue / 3) });
        kindPath.translate(0, kindBaseValue / 2);
        break;
    }
    case ivm::IVInterface::OperationKind::Sporadic: {
        const qreal kindBaseValue = kHeight;
        kindPath.moveTo(-kindBaseValue / 2, 0);
        kindPath.lineTo(0, -kindBaseValue / 4);
        kindPath.lineTo(0, kindBaseValue / 4);
        kindPath.lineTo(kindBaseValue / 2, 0);
        break;
    }
    case ivm::IVInterface::OperationKind::Protected: {
        const qreal kindBaseValue = kHeight;
        const QRectF rect { -kindBaseValue / 2, -kindBaseValue / 2, kindBaseValue, kindBaseValue * 2 / 3 };
        kindPath.addRoundedRect(rect, 2, 2);
        QRectF arcRect(rect.adjusted(rect.width() / 5, 0, -rect.width() / 5, 0));
        arcRect.moveCenter(QPointF(rect.center().x(), rect.top()));
        kindPath.moveTo(arcRect.center());
        kindPath.arcTo(arcRect, 0, 180);
        kindPath.translate(0, rect.height() / 3);
        break;
    }
    default:
        break;
    }
    return kindPath;
}

QPainterPath IVInterfaceGraphicsItem::itemPath(Qt::Alignment alignment) const
{
    QPainterPath path = typeTransform(alignment).map(m_type->path());
    path = path.united(ifaceTransform(alignment).map(m_iface->path()));
    return path;
}

QPainterPath IVInterfaceGraphicsItem::composeShape() const
{
    QPainterPath path = m_textItem->transform().map(m_textItem->shape());
    path.translate(m_textItem->pos());
    for (auto sub : { m_type, m_iface }) {
        QPainterPath subPath = sub->transform().map(sub->path());
        subPath.translate(sub->pos());
        path = path.united(subPath);
    }

    QPainterPath strokeBasePath = m_iface->transform().map(m_iface->path());
    strokeBasePath.translate(m_iface->pos());

    QPainterPathStroker pathStroker;
    pathStroker.setCapStyle(Qt::PenCapStyle::RoundCap);
    pathStroker.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pathStroker.setWidth(shared::graphicsviewutils::kInterfaceBaseLength);
    const QPainterPath strokePath = pathStroker.createStroke(strokeBasePath);
    path = path.united(strokePath);

    return path.simplified();
}

void IVInterfaceGraphicsItem::updateEntityTitle(const QString &text)
{
    const QString newName = ivm::IVNameValidator::encodeName(entity()->type(), text);
    if (newName == entity()->title()) {
        return;
    }

    Q_ASSERT(!m_commandsStack.isNull());
    if (m_commandsStack.isNull()) {
        qWarning() << "Command stack not set for IVInterfaceGraphicsItem";
        return;
    }

    if (!ivm::IVNameValidator::isAcceptableName(entity(), newName)) {
        return;
    }
    const QList<EntityAttribute> attributes = { EntityAttribute {
            ivm::meta::Props::token(ivm::meta::Props::Token::name), newName, EntityAttribute::Type::Attribute } };
    m_commandsStack->push(new shared::cmd::CmdEntityAttributesChange(
            ivm::IVPropertyTemplateConfig::instance(), entity(), attributes));
}
}
