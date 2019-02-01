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

#ifndef DOCUMENTITEM_H
#define DOCUMENTITEM_H

#include <QGraphicsObject>
#include <memory>

namespace msc {

class MscDocument;

class DocumentItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QSizeF boxSize READ boxSize WRITE setBoxSize NOTIFY boxSizeChanged)

public:
    explicit DocumentItem(MscDocument *document, QGraphicsItem *parent = nullptr);
    ~DocumentItem() override;

    QRectF boundingRect() const override;

    void setBoundingRect(const QRectF &rect);

    // Get the size of the written text with this font
    QSizeF textSize(const QFont &font) const;

    QString documentName() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QSizeF boxSize() const;

    void addChildDocument(DocumentItem *item);
    const QVector<DocumentItem *> &childDocuments() const;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public Q_SLOTS:
    void setBoxSize(const QSizeF &size);

Q_SIGNALS:
    void preferredSizeChanged();
    void boxSizeChanged(QSizeF size);
    void doubleClicked(msc::MscDocument *document);

private:
    struct DocumentItemPrivate;
    std::unique_ptr<DocumentItemPrivate> const d;
};
}

#endif // DOCUMENTITEM_H
