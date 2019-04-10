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

#include "hierarchyviewmodel.h"

#include "commands/common/commandsstack.h"
#include "documentitem.h"
#include "mscdocument.h"
#include "mscmodel.h"

#include <QGraphicsScene>
#include <QPointer>

namespace msc {

class HierarchyViewModel::HierarchyViewModelPrivate : public QObject
{
public:
    HierarchyViewModelPrivate(HierarchyViewModel *parent)
        : QObject(parent)
    {
    }
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
            connect(item, &DocumentItem::preferredSizeChanged, this, &HierarchyViewModelPrivate::layoutItems);
            connect(document, &MscDocument::hierarchyTypeChanged, this, &HierarchyViewModelPrivate::layoutItems);

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
        QSizeF totalSize = layoutItemsHelper(topLevelDocumentItems, boxSpacingX, boxSpacingY, 0, boxSize);
        // Add some extra width on the bounding box for drawing outside the box
        totalSize = totalSize + QSizeF(2. * margin, 2. * margin);
        scene.setSceneRect({ { -margin, -margin }, totalSize });

        scene.update();
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

            qreal pathHeight = boxSize.height() + (childrensRect.height() > 0 ? childrensRect.height() + spaceY : 0.);
            height = std::max(height, pathHeight);
            width = x + boundingRect.width();

            // Position of the next box
            x += boundingRect.width() + spaceX;

            item->setBoundingRect(boundingRect);
        }

        return QSizeF(width, height);
    }

    void clear()
    {
        scene.clear();
        documentItems.clear();
        topLevelDocumentItems.clear();

        if (model != nullptr) {
            model->disconnect(this);
        }
    }

    MscModel *model = nullptr;

    QGraphicsScene scene;
    QVector<DocumentItem *> documentItems;
    QVector<DocumentItem *> topLevelDocumentItems;
};

HierarchyViewModel::HierarchyViewModel(QObject *parent)
    : QObject(parent)
    , d(new HierarchyViewModelPrivate(this))
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
        d->clear();

        d->model = model;
        if (model) {
            connect(model, &QObject::destroyed, this, &HierarchyViewModel::modelDeleted);
        }

        updateModel();
    }
}

MscDocument *HierarchyViewModel::selectedDocument() const
{
    for (msc::DocumentItem *item : d->documentItems) {
        if (item->isSelected())
            return item->document();
    }

    return nullptr;
}

void HierarchyViewModel::selectionChanged(const MscDocument *document)
{
    for (msc::DocumentItem *item : d->documentItems) {
        item->setSelected(item->document() == document);
    }
}

void HierarchyViewModel::updateModel()
{
    d->clear();

    if (d->model != nullptr) {
        d->addDocuments(d->model->documents(), nullptr);
        d->layoutItems();

        for (msc::DocumentItem *item : d->documentItems) {
            QObject::connect(item, &msc::DocumentItem::doubleClicked, this,
                             &msc::HierarchyViewModel::documentDoubleClicked, Qt::UniqueConnection);
            QObject::connect(item, &msc::DocumentItem::clicked, this, &msc::HierarchyViewModel::documentClicked,
                             Qt::UniqueConnection);

            QObject::connect(item->document(), &MscDocument::dataChanged, this, &HierarchyViewModel::updateModel,
                             Qt::UniqueConnection);

            QObject::connect(item, &DocumentItem::moved, this, &HierarchyViewModel::documentMoved,
                             Qt::UniqueConnection);
            QObject::connect(item, &DocumentItem::positionChanged, this, &HierarchyViewModel::documentPositionChanged,
                             Qt::UniqueConnection);
        }
    }
}

void HierarchyViewModel::modelDeleted()
{
    d->clear();
    d->model = nullptr;
}

void HierarchyViewModel::documentMoved(const DocumentItem *documentItem, const QPointF &point)
{
    DocumentItem *parentItem = nearestDocumentItem(documentItem, point);

    if (parentItem && parentItem != documentItem->parentItem() && parentItem->document()->isAddChildEnable()) {
        msc::cmd::CommandsStack::push(msc::cmd::MoveDocument,
                                      { QVariant::fromValue<MscDocument *>(documentItem->document()),
                                        QVariant::fromValue<MscDocument *>(parentItem->document()) });
    } else {
        updateModel();
    }
}

void HierarchyViewModel::documentPositionChanged(const QPointF &position)
{
    static QPointer<DocumentItem> documentItem;

    // reset prev document
    if (documentItem) {
        documentItem->setState(DocumentItem::StateCommon);
    }

    auto currentItem = dynamic_cast<DocumentItem *>(sender());
    documentItem = nearestDocumentItem(currentItem, position);

    if (documentItem) {
        documentItem->setState(
                (documentItem != currentItem->parentItem() && documentItem->document()->isAddChildEnable())
                        ? DocumentItem::StateChildEnable
                        : DocumentItem::StateChildDisable);
    }
}

DocumentItem *HierarchyViewModel::nearestDocumentItem(const DocumentItem *documentItem, const QPointF &position)
{
    DocumentItem *nearestItem = nullptr;

    QList<QGraphicsItem *> items = graphicsScene()->items(position);
    for (const auto item : items) {
        if (item != documentItem) {
            nearestItem = dynamic_cast<DocumentItem *>(item);
            break;
        }
    }

    return nearestItem;
}
}
