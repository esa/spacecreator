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

/*!
   \brief DocumentItemModel::index returns the ModelIndex of the given chart
 */
QModelIndex DocumentItemModel::index(MscChart *chart) const
{
    if (!chart || m_mscModel.isNull()) {
        return QModelIndex();
    }

    if (chart->parentDocument()) {
        return createIndex(chart->parentDocument()->charts().indexOf(chart), 0, chart);
    } else {
        return createIndex(m_mscModel->charts().indexOf(chart), 0, chart);
    }
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
        if (m_mscModel->documents().isEmpty()) {
            if (row >= m_mscModel->charts().size()) {
                qWarning() << Q_FUNC_INFO << "row index exceeds charts list";
                return QModelIndex();
            }
            return createIndex(row, column, m_mscModel->charts().at(row));
        } else {
            if (row >= m_mscModel->documents().size()) {
                qWarning() << Q_FUNC_INFO << "row index exceeds documents list";
                return QModelIndex();
            }
            return createIndex(row, column, m_mscModel->documents().at(row));
        }
    } else {
        parentItem = static_cast<MscDocument *>(parent.internalPointer());
    }

    if (!parentItem) {
        return QModelIndex();
    }

    if (parentItem->documents().isEmpty()) {
        if (row >= parentItem->charts().size()) {
            qWarning() << Q_FUNC_INFO << "row index exceeds charts list";
            return QModelIndex();
        }
        return createIndex(row, column, parentItem->charts().at(row));
    } else {
        if (row >= parentItem->documents().size()) {
            qWarning() << Q_FUNC_INFO << "row index exceeds documents list";
            return QModelIndex();
        }
        return createIndex(row, column, parentItem->documents().at(row));
    }
}

QModelIndex DocumentItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || m_mscModel.isNull() || child.internalPointer() == nullptr) {
        return QModelIndex();
    }

    auto obj = static_cast<QObject *>(child.internalPointer());

    auto parentItem = dynamic_cast<MscDocument *>(obj->parent());

    if (!parentItem) {
        return QModelIndex();
    }

    auto documentItem = dynamic_cast<MscDocument *>(parentItem);
    auto mscItem = dynamic_cast<MscChart *>(parentItem);

    if (!documentItem && !mscItem) {
        return QModelIndex();
    }

    auto grandParentItem = dynamic_cast<MscDocument *>(parentItem->parent());
    auto model = dynamic_cast<MscModel *>(parentItem->parent());

    if (grandParentItem) {
        if (documentItem) {
            return createIndex(grandParentItem->documents().indexOf(documentItem), 0, parentItem);
        }
        if (mscItem) {
            return createIndex(grandParentItem->charts().indexOf(mscItem), 0, parentItem);
        }
    }

    if (model) {
        if (documentItem) {
            return createIndex(model->documents().indexOf(documentItem), 0, parentItem);
        }
        if (mscItem) {
            return createIndex(model->charts().indexOf(mscItem), 0, mscItem);
        }
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
        if (m_mscModel->documents().isEmpty()) {
            return m_mscModel->charts().size();
        } else {
            return m_mscModel->documents().size();
        }
    }

    auto obj = static_cast<QObject *>(parent.internalPointer());
    auto documentItem = dynamic_cast<MscDocument *>(obj);
    if (!documentItem) {
        return 0;
    }

    if (documentItem->documents().isEmpty()) {
        return documentItem->charts().size();
    } else {
        return documentItem->documents().size();
    }
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

    if (role == Qt::DecorationRole) {
        auto document = static_cast<MscDocument *>(index.internalPointer());
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
            }
            return QVariant(icon);
        } else {
            return QVariant();
        }
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto document = static_cast<MscDocument *>(index.internalPointer());
        if (document) {
            return QVariant(document->name());
        }
        auto chart = static_cast<MscChart *>(index.internalPointer());
        if (chart) {
            return QVariant(chart->name());
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

    QVariant item;
    auto document = static_cast<MscDocument *>(index.internalPointer());
    if (document) {
        item = QVariant::fromValue(document);
    } else {
        auto chart = static_cast<MscChart *>(index.internalPointer());
        if (!chart) {
            return false;
        }

        item = QVariant::fromValue(chart);
    }

    cmd::CommandsStack::push(cmd::RenameEntity, { item, value });

    return true;
}

void DocumentItemModel::onEntityDataChanged()
{
    auto chart = qobject_cast<msc::MscChart *>(sender());
    if (chart) {
        QModelIndex idx = index(chart);
        if (idx.isValid()) {
            Q_EMIT dataChanged(idx, idx);
        }
    }

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
    connect(document, &msc::MscDocument::documentAdded, this,
            [&]() {
                beginResetModel();
                endResetModel();
            },
            Qt::UniqueConnection);
    connect(document, &msc::MscDocument::chartAdded, this,
            [&]() {
                beginResetModel();
                endResetModel();
            },
            Qt::UniqueConnection);

    connect(document, &msc::MscDocument::hierarchyTypeChanged, this, &msc::DocumentItemModel::onEntityDataChanged,
            Qt::UniqueConnection);

    connect(document, &msc::MscDocument::nameChanged, this, &msc::DocumentItemModel::onEntityDataChanged,
            Qt::UniqueConnection);

    for (auto doc : document->documents()) {
        connectDocument(doc);
    }

    for (auto chart : document->charts()) {
        connect(chart, &msc::MscChart::nameChanged, this, &msc::DocumentItemModel::onEntityDataChanged,
                Qt::UniqueConnection);
    }
}

void DocumentItemModel::disconnectDocument(MscDocument *document)
{
    disconnect(document, nullptr, this, nullptr);

    for (auto doc : document->documents()) {
        disconnectDocument(doc);
    }

    for (auto chart : document->charts()) {
        disconnect(chart, nullptr, this, nullptr);
    }
}

} // namespace msc
