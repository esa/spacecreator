/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include "documentsmanager.h"

namespace taste3 {
namespace document {

DocumentsManager::DocumentsManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent)
    , m_tabs(tabWidget)
{
    connect(m_tabs, &QTabWidget::currentChanged, this, &DocumentsManager::currentDocIdChanged);
}

bool DocumentsManager::addDocument(document::AbstractTabDocument *doc, QWidget *view)
{
    if (m_documents.contains(doc))
        return false;

    m_documents.append(doc);
    m_views.insert(doc, view);

    m_tabs->addTab(view, doc->title());
    return true;
}

int DocumentsManager::docId(document::AbstractTabDocument *doc) const
{
    return doc ? m_documents.indexOf(doc) : InvalidDocId;
}

bool DocumentsManager::isValidDocId(int id) const
{
    return id >= 0 && id < m_documents.size();
}

bool DocumentsManager::removeDocument(int docId)
{
    if (AbstractTabDocument *doc = docById(docId)) {
        return removeDocument(doc);
    }

    return false;
}

bool DocumentsManager::removeDocument(document::AbstractTabDocument *doc)
{
    const int docId = this->docId(doc);
    if (!isValidDocId(docId))
        return false;

    m_documents.remove(docId);
    m_views.remove(doc);
    return true;
}

AbstractTabDocument *DocumentsManager::docById(int id) const
{
    return isValidDocId(id) ? m_documents.at(id) : nullptr;
}

int DocumentsManager::currentDocId() const
{
    return m_tabs->currentIndex();
}

} // ns document
} // ns taste3
