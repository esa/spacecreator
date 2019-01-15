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

#include "mscdocument.h"
#include "mscchart.h"

namespace msc {

MscDocument::MscDocument(QObject *parent)
    : MscEntity(parent)
{
}

MscDocument::MscDocument(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

MscDocument::~MscDocument()
{
    clear();
}

const QVector<MscDocument *> &MscDocument::documents() const
{
    return m_documents;
}

void MscDocument::addDocument(MscDocument *document)
{
    if (document == nullptr) {
        return;
    }
    if (m_documents.contains(document)) {
        return;
    }

    document->setParent(this);
    m_documents.append(document);
    Q_EMIT documentAdded(document);
}

const QVector<MscChart *> &MscDocument::charts() const
{
    return m_charts;
}

void MscDocument::addChart(MscChart *chart)
{
    if (chart == nullptr) {
        return;
    }
    if (m_charts.contains(chart)) {
        return;
    }

    chart->setParent(this);
    m_charts.append(chart);
    Q_EMIT chartAdded(chart);
}

void MscDocument::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    Q_EMIT cleared();
}

/*!
  \brief MscDocument::parentDocument
  \return The document that this one is included in. Returns a nullptr if this document is at the
  root
*/
MscDocument *MscDocument::parentDocument() const
{
    return dynamic_cast<MscDocument *>(parent());
}

MscEntity::EntityType MscDocument::entityType() const
{
    return MscEntity::EntityType::Document;
}

MscDocument::HierarchyType MscDocument::hierarchyType() const
{
    return m_hierarchyType;
}

void MscDocument::setHierarchyType(MscDocument::HierarchyType type)
{
    if (type != m_hierarchyType) {
        m_hierarchyType = type;
        Q_EMIT hierarchyTypeChanged(type);
    }
}

}
