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

#include <QPointer>
#include <QPainter>
#include <QDebug>

namespace msc {

struct DocumentItem::DocumentItemPrivate {
    QPointer<msc::MscDocument> document;
    QSizeF boxSize;
    QVector<DocumentItem *> childDocuments;
    QRectF boundingRect;
};

DocumentItem::DocumentItem(MscDocument *document, QGraphicsItem *parent)
    : QGraphicsObject(parent), d(new DocumentItemPrivate)
{
    d->document = document;
    d->boxSize.setWidth(100);
    d->boxSize.setHeight(50);
}

DocumentItem::~DocumentItem()
{
}

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
    QRectF boxRect;
    boxRect.setX((boundingRect().width() - boxSize().width()) / 2);
    boxRect.setSize(boxSize());

    QPen pen(Qt::black);
    painter->setPen(pen);
    painter->fillRect(boxRect, Qt::white);
    painter->drawRect(boxRect);
    painter->drawText(boxRect, Qt::AlignCenter, d->document->name());
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
