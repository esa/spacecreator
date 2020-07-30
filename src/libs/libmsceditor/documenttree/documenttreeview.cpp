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

#include "documenttreeview.h"

#include "documentitemmodel.h"
#include "mscdocument.h"

#include <QLineEdit>
#include <QMenu>
#include <QModelIndexList>
#include <QRegExp>
#include <QRegExpValidator>
#include <QScopedPointer>
#include <QStyledItemDelegate>
#include <functional>

namespace msc {
static const char *HIERARCHY_TYPE_TAG { "hierarchyTag" };

class DocumentDelegate : public QStyledItemDelegate
{
public:
    DocumentDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
        auto line = qobject_cast<QLineEdit *>(editor);
        if (line) {
            auto nameValidator = new QRegExpValidator(msc::MscEntity::nameVerifier(), line);
            line->setValidator(nameValidator);
        }
        return editor;
    }
};

/*!
 * \class msc::DocumentTreeView The class for showing the document tree in the main window
 */

/*!
 * \brief DocumentTreeView::DocumentTreeView Constructor
 * \param parent Widget parent
 */
DocumentTreeView::DocumentTreeView(QWidget *parent)
    : QTreeView(parent)
{
    expandAll();
    setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);

    setItemDelegate(new DocumentDelegate(this));

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTreeView::customContextMenuRequested, this, &DocumentTreeView::showDocumentViewMenu);
}

void DocumentTreeView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    connect(model, &QAbstractItemModel::modelReset, this, &QTreeView::expandAll);
    static bool connected = false;
    if (!connected) {
        connect(selectionModel(), &QItemSelectionModel::selectionChanged, this,
                [&]() { Q_EMIT selectedDocumentChanged(selectedDocument()); });
        connected = true;
    }
}

/*!
 * \brief DocumentTreeView::currentDocument Get the current MSC document
 * \return the current document
 */
msc::MscDocument *DocumentTreeView::currentDocument() const
{
    auto *obj = static_cast<QObject *>(currentIndex().internalPointer());
    if (obj == nullptr)
        return nullptr;

    return qobject_cast<msc::MscDocument *>(obj);
}

/*!
 * \brief DocumentTreeView::setSelectedDocument Choose the current document
 * \param document The document to be selected
 */
void DocumentTreeView::setSelectedDocument(msc::MscDocument *document)
{
    if (document == selectedDocument())
        return;

    std::function<void(int, const QModelIndex &)> findDocument;

    findDocument = [&](int row, const QModelIndex &parent) -> void {
        QModelIndex index = model()->index(row, 0, parent);

        if (index.internalPointer() == document) {
            selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
        }

        for (int x = 0; x < model()->rowCount(index); ++x) {
            findDocument(x, index);
        }
    };

    for (int row = 0; row < model()->rowCount(); ++row) {
        findDocument(row, QModelIndex());
    }
}

/*!
 * \brief DocumentTreeView::selectedDocument Get the selected document
 * \return The current document or null if none is selected
 */
msc::MscDocument *DocumentTreeView::selectedDocument() const
{
    QModelIndexList selection = selectedIndexes();
    if (selection.isEmpty())
        return nullptr;

    auto *obj = static_cast<QObject *>(selection.at(0).internalPointer());
    if (obj == nullptr)
        return nullptr;

    return qobject_cast<msc::MscDocument *>(obj);
}

QSize DocumentTreeView::sizeHint() const
{
    return QSize(200, 200);
}

/*!
 * \brief DocumentTreeView::changHierarchyType Update the hierarchy type
 */
void DocumentTreeView::changHierarchyType()
{
    auto docModel = static_cast<msc::DocumentItemModel *>(model());
    docModel->updateHierarchyType(currentIndex(), sender()->property(HIERARCHY_TYPE_TAG));
}

/*!
 * \brief DocumentTreeView::showDocumentViewMenu Show the context menu
 * \param point Show at this point
 */
void DocumentTreeView::showDocumentViewMenu(const QPoint &point)
{
    QModelIndex index = indexAt(point);

    if (index.isValid()) {
        auto *obj = static_cast<QObject *>(index.internalPointer());

        msc::MscDocument *document = dynamic_cast<msc::MscDocument *>(obj);
        if (document) {
            QScopedPointer<QMenu> documentViewMenu(contextMenu(document));
            documentViewMenu->exec(viewport()->mapToGlobal(point));
        }
    }
}

/*!
 * \brief DocumentTreeView::contextMenu Get a context menu
 * \param document The document chosen to show the menu for
 * \return The new menu. The caller must delete this menu.
 */
QMenu *DocumentTreeView::contextMenu(msc::MscDocument *document)
{
    QMenu *menu = new QMenu(this);

    auto addAction = [&](const QString &icon, const QString &text, msc::MscDocument::HierarchyType type) {
        auto action = new QAction(QIcon(icon), text, this);
        action->setProperty(HIERARCHY_TYPE_TAG, type);
        action->setEnabled(type != document->hierarchyType());
        if (type != document->hierarchyType()) {
            if (!document->charts().isEmpty()) {
                // document has charts - possible only type leaf
                action->setDisabled(type == msc::MscDocument::HierarchyType::HierarchyLeaf);
            } else {
                if (type == msc::MscDocument::HierarchyType::HierarchyLeaf)
                    action->setDisabled(!document->documents().isEmpty());
                else
                    // constraint for "repeat", "is", "leaf" and "exception" - possible if only one child
                    action->setDisabled(document->documents().size() > 1
                            && (type == msc::MscDocument::HierarchyType::HierarchyRepeat
                                    || type == msc::MscDocument::HierarchyType::HierarchyIs
                                    || type == msc::MscDocument::HierarchyType::HierarchyException));
            }
        }

        connect(action, &QAction::triggered, this, &DocumentTreeView::changHierarchyType);

        menu->addAction(action);
    };

    addAction(":/icons/document_and.png", tr("Hierarchy And"), msc::MscDocument::HierarchyAnd);
    addAction(":/icons/document_or.png", tr("Hierarchy Or"), msc::MscDocument::HierarchyOr);
    addAction(":/icons/document_parallel.png", tr("Hierarchy Parallel"), msc::MscDocument::HierarchyParallel);
    addAction(":/icons/document_is_scenario.png", tr("Hierarchy Is"), msc::MscDocument::HierarchyIs);
    addAction(":/icons/document_repeat.png", tr("Hierarchy Repeat"), msc::MscDocument::HierarchyRepeat);
    addAction(":/icons/document_exception.png", tr("Hierarchy Exception"), msc::MscDocument::HierarchyException);
    addAction(":/icons/document_leaf.png", tr("Hierarchy Leaf"), msc::MscDocument::HierarchyLeaf);

    menu->addSeparator();
    menu->addAction(tr("Rename"), this, [&]() { edit(currentIndex()); });

    return menu;
}

}
