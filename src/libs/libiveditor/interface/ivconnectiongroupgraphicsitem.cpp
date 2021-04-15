/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivconnectiongroupgraphicsitem.h"

#include "ivconnectiongroup.h"
#include "ivinterfacegroupgraphicsitem.h"

#include <QFontMetrics>

const qreal kConnectionGroupTitleMaxLength = 200;

namespace ive {

IVConnectionGroupGraphicsItem::IVConnectionGroupGraphicsItem(ivm::IVConnectionGroup *connection,
        IVInterfaceGroupGraphicsItem *ifaceStart, IVInterfaceGroupGraphicsItem *ifaceEnd, QGraphicsItem *parent)
    : IVConnectionGraphicsItem(connection, ifaceStart, ifaceEnd, parent)
    , m_textItem(new QGraphicsTextItem(this))
{
    connect(connection, &ivm::IVObject::titleChanged, this, &IVConnectionGroupGraphicsItem::updateLabel);
}

shared::ColorManager::HandledColors IVConnectionGroupGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::ConnectionGroup;
}

void IVConnectionGroupGraphicsItem::init()
{
    IVConnectionGraphicsItem::init();
    updateLabel(entity()->titleUI());
}

void IVConnectionGroupGraphicsItem::updateBoundingRect()
{
    IVConnectionGraphicsItem::updateBoundingRect();
    updateTitleItemPosition();
}

void IVConnectionGroupGraphicsItem::updateLabel(const QString &text)
{
    if (text != m_textItem->toPlainText()) {
        const QFontMetrics fm(m_textItem->font());
        m_textItem->setPlainText(fm.elidedText(text, Qt::ElideRight, kConnectionGroupTitleMaxLength));
        instantLayoutUpdate();
    }
}

void IVConnectionGroupGraphicsItem::updateTitleItemPosition()
{
    const QVector<QPointF> scenePoints = points();
    const QString text = m_textItem->toPlainText().trimmed();
    if (scenePoints.size() < 2 || text.isEmpty())
        return;

    const qreal width = m_textItem->boundingRect().width();
    const qreal height = m_textItem->boundingRect().height();
    QLineF longest, horizontal;
    for (int idx = 1; idx < scenePoints.size(); ++idx) {
        const QLineF line { scenePoints.at(idx - 1), scenePoints.at(idx) };
        if (line.length() > longest.length()) {
            longest = line;
        }
        if (qAbs(qRound(line.angle()) % 180) < qAbs(qRound(horizontal.angle()) % 180)) {
            horizontal = line;
        }
    }
    if (horizontal.length() >= width) {
        longest = horizontal;
    }
    QRectF textRect = m_textItem->boundingRect();
    textRect.moveCenter(longest.center());
    QLineF normal = longest.normalVector();
    normal.translate(textRect.center() - normal.p1());
    QTransform transform;
    transform.translate(width / 2, height / 2);
    if (longest.angle() > 90 && longest.angle() < 270) {
        normal.setAngle(normal.angle() + 180);
        transform.rotate(-(longest.angle() + 180));
    } else {
        transform.rotate(-longest.angle());
    }
    transform.translate(-width / 2, -height / 2);
    m_textItem->setTransform(transform);
    normal.setLength(height / 2);
    textRect.moveCenter(normal.p2());

    const QPointF topLeft = mapFromScene(textRect.topLeft());
    m_textItem->setPos(topLeft);
}

} // namespace ive
