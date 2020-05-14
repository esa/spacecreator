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

#include "documentitemmodel.h"

#include "commands/common/commandsstack.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmodel.h"

#include <QDebug>
#include <QPixmap>

namespace msc {

/*!
  \class msc::DocumentItemModel
  \brief ItemModel of the documents and charts to be used in a QAbstractItemView

  \inmodule MscLibrary
*/

DocumentItemModel::DocumentItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    connect(
            this, &QAbstractItemModel::modelReset, this,
            [&]() {
                for (auto doc : m_mscModel->documents()) {
                    connectDocument(doc);
                }
            },
            Qt::QueuedConnection);
}

/*!
  \brief Set the msc::MscModel to be shown
  \param model
*/
void DocumentItemModel::setMscModel(msc::MscModel *model)
{
    if (model == m_mscModel.data()) {
        return;
    }

    beginResetModel();
    if (!m_mscModel.isNull()) {
        for (auto doc : m_mscModel->documents()) {
            disconnectDocument(doc);
        }
        disconnect(m_mscModel, nullptr, this, nullptr);
    }
    m_mscModel = model;
    if (m_mscModel) {
        connect(m_mscModel, &QObject::destroyed, this, [&]() { setMscModel(nullptr); });
        connect(m_mscModel, &msc::MscModel::documentAdded, this, [&]() {
            beginResetModel();
            endResetModel();
        });
        connect(m_mscModel, &msc::MscModel::chartAdded, this, [&]() {
            beginResetModel();
            endResetModel();
        });

        for (auto doc : m_mscModel->documents()) {
            connectDocument(doc);
        }
    }
    endResetModel();
}

MscModel *DocumentItemModel::mscModel() const
{
    return m_mscModel.data();
}

QModelIndex DocumentItemModel::index(MscDocument *document) const
{
    if (!document || m_mscModel.isNull()) {
        return QModelIndex();
    }

    if (document->parentDocument()) {
        return createIndex(document->parentDocument()->documents().indexOf(document), 0, document);
    } else {
        return createIndex(m_mscModel->documents().indexOf(document), 0, document);
    }
}

/*!
 * \brief DocumentItemModel::updateHierarchyType Set a hierarchy type on an entry
 * \param index the index of the entry
 * \param hierarchyType the new hierarchy
 */
void DocumentItemModel::updateHierarchyType(const QModelIndex &index, const QVariant &hierarchyType)
{
    if (!index.isValid() || index.internalPointer() == nullptr) {
        return;
    }

    auto obj = static_cast<QObject *>(index.internalPointer());
    if (auto document = dynamic_cast<MscDocument *>(obj)) {
        cmd::CommandsStack::push(cmd::HierarchyType, { QVariant::fromValue(document), hierarchyType });
    }
}

Qt::ItemFlags DocumentItemModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex DocumentItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0 || m_mscModel.isNull()) {
        return QModelIndex();
    }

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    MscDocument *parentItem = nullptr;

    if (!parent.isValid()) {
        if (row >= m_mscModel->documents().size()) {
            qWarning() << Q_FUNC_INFO << "row index exceeds documents list";
            return QModelIndex();
        }
        return createIndex(row, column, m_mscModel->documents().at(row));
    } else {
        parentItem = static_cast<MscDocument *>(parent.internalPointer());
    }

    if (!parentItem) {
        return QModelIndex();
    }

    if (row >= parentItem->documents().size()) {
        qWarning() << Q_FUNC_INFO << "row index exceeds documents list";
        return QModelIndex();
    }

    return createIndex(row, column, parentItem->documents().at(row));
}

QModelIndex DocumentItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || m_mscModel.isNull() || child.internalPointer() == nullptr) {
        return QModelIndex();
    }

    auto obj = static_cast<QObject *>(child.internalPointer());

    MscDocument *parentItem;
    if (!obj || !(parentItem = dynamic_cast<MscDocument *>(obj->parent()))) {
        return QModelIndex();
    }

    auto documentItem = dynamic_cast<MscDocument *>(parentItem);

    if (!documentItem) {
        return QModelIndex();
    }

    auto grandParentItem = dynamic_cast<MscDocument *>(parentItem->parent());
    auto model = dynamic_cast<MscModel *>(parentItem->parent());

    if (grandParentItem && documentItem) {
        return createIndex(grandParentItem->documents().indexOf(documentItem), 0, parentItem);
    }

    if (model && documentItem) {
        return createIndex(model->documents().indexOf(documentItem), 0, parentItem);
    }

    return QModelIndex();
}

int DocumentItemModel::rowCount(const QModelIndex &parent) const
{
    if (m_mscModel.isNull()) {
        return 0;
    }

    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        return m_mscModel->documents().size();
    }

    auto obj = static_cast<QObject *>(parent.internalPointer());
    auto documentItem = dynamic_cast<MscDocument *>(obj);
    if (!documentItem) {
        return 0;
    }

    return documentItem->documents().size();
}

int DocumentItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant DocumentItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    auto entity = static_cast<MscEntity *>(index.internalPointer());

    if (role == Qt::DecorationRole) {
        auto document = dynamic_cast<MscDocument *>(entity);
        if (document) {
            QPixmap icon;
            switch (document->hierarchyType()) {
            case msc::MscDocument::HierarchyLeaf:
                icon = QPixmap(":/icons/document_leaf.png");
                break;
            case msc::MscDocument::HierarchyIs:
                icon = QPixmap(":/icons/document_is_scenario.png");
                break;
            case msc::MscDocument::HierarchyAnd:
                icon = QPixmap(":/icons/document_and.png");
                break;
            case msc::MscDocument::HierarchyOr:
                icon = QPixmap(":/icons/document_or.png");
                break;
            case msc::MscDocument::HierarchyParallel:
                icon = QPixmap(":/icons/document_parallel.png");
                break;
            case msc::MscDocument::HierarchyRepeat:
                icon = QPixmap(":/icons/document_repeat.png");
                break;
            case msc::MscDocument::HierarchyException:
                icon = QPixmap(":/icons/document_exception.png");
                break;
            default:
                break;
            }
            return QVariant(icon);
        } else {
            return QVariant();
        }
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (auto document = dynamic_cast<MscDocument *>(entity)) {
            return QVariant(document->name());
        }
    }

    return QVariant();
}

QVariant DocumentItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return QVariant(tr("Name"));
    }

    return QVariant();
}

bool DocumentItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    if (auto document = static_cast<MscDocument *>(index.internalPointer())) {
        QVariant item;
        item = QVariant::fromValue(document);

        cmd::CommandsStack::push(cmd::RenameEntity, { item, value });
    }

    return true;
}

/*!
 * \brief DocumentItemModel::onEntityDataChanged Update the index when data changes
 */
void DocumentItemModel::onEntityDataChanged()
{
    auto doc = qobject_cast<msc::MscDocument *>(sender());
    if (doc) {
        QModelIndex idx = index(doc);
        if (idx.isValid()) {
            Q_EMIT dataChanged(idx, idx);
        }
    }
}

void DocumentItemModel::connectDocument(MscDocument *document)
{
    disconnectDocument(document);

    connect(document, &msc::MscDocument::documentAdded, this, [&]() {
        beginResetModel();
        endResetModel();
    });
    connect(document, &msc::MscDocument::documentRemoved, this, [&]() {
        beginResetModel();
        endResetModel();
    });

    connect(document, &msc::MscDocument::hierarchyTypeChanged, this, &msc::DocumentItemModel::onEntityDataChanged,
            Qt::QueuedConnection);

    connect(document, &msc::MscDocument::nameChanged, this, &msc::DocumentItemModel::onEntityDataChanged,
            Qt::QueuedConnection);

    for (auto doc : document->documents()) {
        connectDocument(doc);
    }
}

void DocumentItemModel::disconnectDocument(MscDocument *document)
{
    disconnect(document, nullptr, this, nullptr);

    for (auto doc : document->documents()) {
        disconnectDocument(doc);
    }
}

} // namespace msc
