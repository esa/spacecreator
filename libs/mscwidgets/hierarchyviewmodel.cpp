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

#include "hierarchyviewmodel.h"

#include "mscmodel.h"
#include "mscdocument.h"
#include "documentitem.h"

#include <QGraphicsScene>
#include <QDebug>

namespace msc {

struct HierarchyViewModel::HierarchyViewModelPrivate {
    void addDocuments(const QVector<MscDocument *> &documents, DocumentItem *parentItem)
    {
        for (MscDocument *document : documents) {
            auto item = new DocumentItem(document, parentItem);
            this->documentItems << item;
            if (parentItem == nullptr) {
                this->scene.addItem(item);
                this->topLevelDocumentItems << item;
            } else {
                parentItem->addChildDocument(item);
            }

            addDocuments(document->documents(), item);
        }
    }

    void layoutItems()
    {
        // First find the proper size of each box based on the text contents
        const QSizeF maxSize = maxDocumentsTextSize();

        // Set the box size to be slightly larger than the necessary text size
        const qreal margin = maxSize.height();
        const QSizeF boxSize(maxSize.width() + margin, maxSize.height() + margin);

        // Now layout the items with child sizes
        const qreal boxSpacingX = maxSize.width() / 3;
        const qreal boxSpacingY = margin * 1.5;
        layoutItemsHelper(topLevelDocumentItems, boxSpacingX, boxSpacingY, 0, boxSize);
    }

    QSizeF maxDocumentsTextSize() const
    {
        QSizeF max(0, 0);
        auto font = scene.font();
        for (auto item : documentItems) {
            QSizeF size = item->textSize(font);
            if (size.width() > max.width()) {
                max.setWidth(size.width());
            }
            if (size.height() > max.height()) {
                max.setHeight(size.height());
            }
        }
        return max;
    }

    // The posY is 0 for the toplevel items and boxSize height + spaceY for all others
    QSizeF layoutItemsHelper(const QVector<DocumentItem *> &items, qreal spaceX, qreal spaceY, qreal posY,
                             const QSizeF &boxSize) const
    {
        qreal x = 0;
        qreal height = 0;
        qreal width = 0;
        for (auto item : items) {
            // First, layout the children and get the size of those
            const QSizeF childrensRect =
                    layoutItemsHelper(item->childDocuments(), spaceX, spaceY, spaceY + boxSize.height(), boxSize);

            item->setX(x);
            item->setY(posY);
            item->setBoxSize(boxSize);

            QRectF boundingRect(0, 0, qMax(boxSize.width(), childrensRect.width()),
                                qMax(posY + boxSize.height(), childrensRect.height()));

            if (boundingRect.height() > height) {
                height = boundingRect.height();
            }

            width = x + boundingRect.width();

            // Position of the next box
            x += boundingRect.width() + spaceX;

            // Add some extra width on the bounding box for drawing outside the box
            boundingRect.setWidth(boundingRect.width() + boxSize.height());
            item->setBoundingRect(boundingRect);
        }

        return QSizeF(width, height);
    }

    void clearScene()
    {
        scene.clear();
        documentItems.clear();
        topLevelDocumentItems.clear();
    }

    MscModel *model = nullptr;

    QGraphicsScene scene;
    QVector<DocumentItem *> documentItems;
    QVector<DocumentItem *> topLevelDocumentItems;
};

HierarchyViewModel::HierarchyViewModel(QObject *parent)
    : QObject(parent)
    , d(new HierarchyViewModelPrivate)
{
}

HierarchyViewModel::~HierarchyViewModel() {}

QGraphicsScene *HierarchyViewModel::graphicsScene() const
{
    return &d->scene;
}

void HierarchyViewModel::setModel(MscModel *model)
{
    if (model != d->model) {
        if (d->model != nullptr) {
            // Remove the old connections and clear the current scene
            d->clearScene();

            d->model->disconnect(this);
        }

        d->model = model;
        connect(model, &QObject::destroyed, this, &HierarchyViewModel::modelDeleted);

        d->addDocuments(model->documents(), nullptr);
        d->layoutItems();

        for (msc::DocumentItem *item : d->documentItems) {
            QObject::connect(item, &msc::DocumentItem::doubleClicked, this,
                             &msc::HierarchyViewModel::documentDoubleClicked);
        }
    }
}

void HierarchyViewModel::modelDeleted()
{
    d->clearScene();
    d->model = nullptr;
}

}
