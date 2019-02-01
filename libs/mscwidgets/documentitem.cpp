/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "documentitem.h"

#include "mscdocument.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointer>

namespace msc {

struct DocumentItem::DocumentItemPrivate {
    QPointer<msc::MscDocument> document;
    QSizeF boxSize;
    QVector<DocumentItem *> childDocuments;
    QRectF boundingRect;
};

DocumentItem::DocumentItem(MscDocument *document, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , d(new DocumentItemPrivate)
{
    d->document = document;
    d->boxSize.setWidth(100);
    d->boxSize.setHeight(50);

    connect(document, &msc::MscDocument::nameChanged, this, [&]() { update(); });
    connect(document, &msc::MscDocument::nameChanged, this, &DocumentItem::preferredSizeChanged);

    setFlag(QGraphicsItem::ItemIsSelectable);
}

DocumentItem::~DocumentItem() {}

QRectF DocumentItem::boundingRect() const
{
    return d->boundingRect;
}

void DocumentItem::setBoundingRect(const QRectF &rect)
{
    d->boundingRect = rect;
}

QSizeF DocumentItem::textSize(const QFont &font) const
{
    QSizeF size;
    if (!d->document.isNull()) {
        QFontMetricsF fm(font);
        size = fm.size(0, d->document->name());
    }
    return size;
}

QString DocumentItem::documentName() const
{
    if (d->document.isNull()) {
        return QString();
    }
    return d->document->name();
}

void DocumentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    if (d->document.isNull()) {
        // Can't paint without a valid document
        return;
    }

    // Calculate the size and position of the boxes
    QRectF boxRect, relationRect;
    boxRect.setX((boundingRect().width() - boxSize().width()) / 2);
    boxRect.setSize(boxSize());
    qreal relationHeight = boxRect.height() / 5;
    qreal relationWidth = relationHeight * 3;
    relationRect.setX(boxRect.center().x() - relationWidth / 2);
    relationRect.setHeight(relationHeight);
    relationRect.setWidth(relationWidth);
    relationRect.moveBottom(boxRect.bottom());
    boxRect.setHeight(boxRect.height() - relationHeight);

    // This is for the connecting lines below. Can be changed for non-leaf relations
    int startLineY = qRound(boxRect.bottom());

    QPen pen(Qt::black);

    if (isSelected()) {
        pen.setWidth(3);
    }

    painter->setBrush(Qt::white);
    painter->setPen(pen);
    painter->drawRect(boxRect);

    if (isSelected()) {
        painter->setPen(QPen(Qt::black));
    }

    painter->drawText(boxRect, Qt::AlignCenter, d->document->name());
    if (d->document->hierarchyType() != MscDocument::HierarchyLeaf) {
        startLineY = qRound(relationRect.bottom());

        painter->drawRect(relationRect);

        painter->setRenderHint(QPainter::Antialiasing);
        const qreal xCenter = relationRect.center().x();
        const qreal yCenter = relationRect.center().y();
        const qreal diff = (yCenter - relationRect.top()) / 2;
        qreal x1 = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0, y1 = 0, y2 = 0;
        switch (d->document->hierarchyType()) {
        case MscDocument::HierarchyLeaf:
            // Handled in a separate else below, because this should not have a box
            break;
        case MscDocument::HierarchyIs:
            x1 = xCenter - diff;
            x2 = xCenter + diff;
            y1 = yCenter - diff / 2;
            y2 = yCenter + diff / 2;
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y1));
            painter->drawLine(QPointF(x1, y2), QPointF(x2, y2));
            break;
        case MscDocument::HierarchyAnd:
            x1 = xCenter - diff;
            x2 = xCenter + diff;
            painter->drawLine(QPointF(x1, yCenter), QPointF(x2, yCenter));
            break;
        case MscDocument::HierarchyOr:
            x1 = xCenter - diff;
            x2 = xCenter + diff;
            y1 = yCenter - diff;
            y2 = yCenter + diff;
            painter->drawLine(QPointF(xCenter, y1), QPointF(xCenter, y2));
            painter->drawLine(QPointF(x1, yCenter), QPointF(x2, yCenter));
            break;
        case MscDocument::HierarchyParallel:
            x1 = xCenter - diff;
            x2 = xCenter + diff;
            y1 = yCenter - diff;
            y2 = yCenter + diff;
            painter->drawLine(QPointF(x1, y1), QPointF(x1, y2));
            painter->drawLine(QPointF(x2, y1), QPointF(x2, y2));
            break;
        case MscDocument::HierarchyRepeat:
            x1 = boxRect.right();
            x5 = relationHeight * 0.7;
            painter->drawEllipse(QPointF(x1, boxRect.center().y()), x5, x5);
            break;
        case MscDocument::HierarchyException:
            x1 = xCenter - diff;
            x2 = xCenter - diff / 2;
            x3 = xCenter;
            x4 = xCenter + diff / 2;
            x5 = xCenter + diff;
            y1 = yCenter - diff;
            y2 = yCenter + diff;
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
            painter->drawLine(QPointF(x2, y2), QPointF(x3, y1));
            painter->drawLine(QPointF(x3, y1), QPointF(x4, y2));
            painter->drawLine(QPointF(x4, y2), QPointF(x5, y1));
            break;
        }
        painter->setRenderHint(QPainter::Antialiasing, false);
    } else {
        // Paint the leaf diagonal line
        painter->drawLine(QPointF(boxRect.x() + 1, boxRect.center().y()),
                          QPointF(boxRect.x() + (boxRect.center().y() - boxRect.y()) + 1, boxRect.bottom()));
    }

    // Now draw the connections
    if (!d->childDocuments.isEmpty()) {
        const int startLineX = qRound(boxRect.center().x());
        const int endLineY = qRound(d->childDocuments.at(0)->y());

        if (d->childDocuments.count() == 1) {
            painter->drawLine(startLineX, startLineY, startLineX, endLineY);
        } else {
            const int midLineY = qRound(boxRect.bottom() + (endLineY - boxRect.bottom()) / 2);

            // Paint the line down to the horizontal line
            painter->drawLine(startLineX, startLineY, startLineX, midLineY);

            auto boxX = [=](int boxIndex) {
                auto item = d->childDocuments.at(boxIndex);
                // The position of the child box is at the center of it's bounding rect
                return qRound(item->boundingRect().center().x() + item->x());
            };

            // Paint the horizontal line
            const int lastIndex = d->childDocuments.count() - 1;
            painter->drawLine(boxX(0), midLineY, boxX(lastIndex), midLineY);

            // Paint the lines from the horizontal line to the child boxes
            for (int i = 0; i <= lastIndex; ++i) {
                const int x = boxX(i);
                painter->drawLine(x, midLineY, x, endLineY);
            }
        }
    }
}

QSizeF DocumentItem::boxSize() const
{
    return d->boxSize;
}

void DocumentItem::addChildDocument(DocumentItem *item)
{
    d->childDocuments << item;
}

const QVector<DocumentItem *> &DocumentItem::childDocuments() const
{
    return d->childDocuments;
}

void DocumentItem::setBoxSize(const QSizeF &size)
{
    d->boxSize = size;
    Q_EMIT boxSizeChanged(size);
}
}

void msc::DocumentItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    Q_EMIT doubleClicked(d->document.data());
}
