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

#include "mscdocument.h"

#include "mscchart.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"

namespace msc {

/*!
 * \class MscDocument
 *
 * This is the main document class.
 */

MscDocument::MscDocument(QObject *parent)
    : MscDocument(DefaultName, parent)
{
}

MscDocument::MscDocument(const QString &name, QObject *parent)
    : MscEntity(name, parent)
    , m_messageDeclarations(new MscMessageDeclarationList(this))
{
    connect(m_messageDeclarations, &MscMessageDeclarationList::dataChanged, this, &MscDocument::dataChanged);
    connect(m_messageDeclarations, &MscMessageDeclarationList::countChanged, this, &MscDocument::dataChanged);
}

MscDocument::~MscDocument()
{
    clear();
}

/*!
 * \brief MscDocument::documents Get the list of child documents
 * \return
 */
const QVector<MscDocument *> &MscDocument::documents() const
{
    return m_documents;
}

/*!
   Adds a document event and takes over parentship.
 */
bool MscDocument::addDocument(MscDocument *document, int index)
{
    return addDocument(document, true, index);
}

/*!
   Adds a document event and takes over parentship.
 */
bool MscDocument::addDocument(MscDocument *document, bool checkType, int index)
{
    if (document == nullptr) {
        return false;
    }
    if (m_documents.contains(document)) {
        return false;
    }

    if (checkType) {
        if (isSingleChildDocument() && m_documents.size() >= 1) {
            return false;
        }
        if (m_hierarchyType == HierarchyLeaf) {
            return false;
        }
    }

    document->setParent(this);

    if (index < 0 || index >= m_documents.size()) {
        m_documents.append(document);
    } else {
        m_documents.insert(index, document);
    }

    connect(document, &MscDocument::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT documentAdded(document);
    Q_EMIT documentsChanged();
    Q_EMIT dataChanged();

    return true;
}

/*!
 * \brief MscDocument::removeDocument Removes a sub document without deleting it
 * \param document
 * \param clear
 */
void MscDocument::removeDocument(MscDocument *document, bool clear)
{
    if (document == nullptr) {
        return;
    }
    if (!m_documents.contains(document)) {
        return;
    }

    if (m_documents.removeAll(document) && clear) {
        document->clear();
    }

    Q_EMIT documentRemoved(document);
    Q_EMIT documentsChanged();
    Q_EMIT dataChanged();
}

/*!
 * \brief MscDocument::charts Get the charts in this document
 * \return
 */
const QVector<MscChart *> &MscDocument::charts() const
{
    return m_charts;
}

/*!
   Adds a chart and takes over parentship.
 */
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
    connect(chart, &MscChart::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT chartAdded(chart);
    Q_EMIT chartsChanged();
    Q_EMIT dataChanged();
}

/*!
   \return The message declarations of the document head
 */
MscMessageDeclarationList *MscDocument::messageDeclarations() const
{
    return m_messageDeclarations;
}

MscMessageDeclarationList *MscDocument::messageDeclarations()
{
    return m_messageDeclarations;
}

/**
    Return the message declarations in a QList (if QObjectListT can't be handeled)
    Normally messageDeclarations() is preferred.
 */
QList<msc::MscMessageDeclaration *> MscDocument::declarations() const
{
    return m_messageDeclarations->objectList();
}

/*!
 * \brief MscDocument::clear Remove all documents and charts
 */
void MscDocument::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    clearCharts();

    {
        QSignalBlocker sb(this);
        m_messageDeclarations->clear(true);
    }

    Q_EMIT cleared();
    Q_EMIT documentsChanged();
    Q_EMIT dataChanged();
}

/*!
 * \brief MscDocument::clearCharts Clear all charts
 */
void MscDocument::clearCharts()
{
    qDeleteAll(m_charts);
    m_charts.clear();
    Q_EMIT chartsChanged();
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

/*!
 * \brief MscDocument::hierarchyType Get the hierarchy type of this document
 * \return
 */
MscDocument::HierarchyType MscDocument::hierarchyType() const
{
    return m_hierarchyType;
}

/*!
 * \brief MscDocument::setHierarchyType Set the hierarchy type of this document
 * \param type
 */
void MscDocument::setHierarchyType(MscDocument::HierarchyType type)
{
    if (type != m_hierarchyType) {
        m_hierarchyType = type;
        Q_EMIT hierarchyTypeChanged(type);
    }
}

/**
   Returns the hierarchy type as short string. As used in MSC
 */
QString MscDocument::hierarchyTypeString() const
{
    switch (m_hierarchyType) {
    case HierarchyLeaf:
        return { "LEAF" };
    case HierarchyIs:
        return { "IS" };
    case HierarchyAnd:
        return { "AND" };
    case HierarchyOr:
        return { "OR" };
    case HierarchyParallel:
        return { "PARALLEL" };
    case HierarchyRepeat:
        return { "REPEAT" };
    case HierarchyException:
        return { "EXCEPTION" };
    default:
        Q_ASSERT(true);
    }
    return {};
}

/*!
   \brief MscDocument::isSingleChildDocument
   \return Returns if this document should only have one single child (dependent on the hierarchy type)
 */
bool MscDocument::isSingleChildDocument() const
{
    return m_hierarchyType == HierarchyIs || m_hierarchyType == HierarchyRepeat
            || m_hierarchyType == HierarchyException;
}

/*!
 * \brief MscDocument::isAddChildEnable Check if a child can be added
 * \return true if a child can be added
 */
bool MscDocument::isAddChildEnable() const
{
    return !(isSingleChildDocument() && m_documents.size() >= 1) && m_hierarchyType != HierarchyLeaf;
}

/*!
   Looks for messageInstanceName and timerInstanceName and if they are numbers, returns the highest number
   Returns 1 as a minimum. Even if nothing was found.
 */
int MscDocument::maxInstanceNameNumber() const
{
    int num = 1;
    for (MscChart *chart : m_charts) {
        num = std::max(num, chart->maxInstanceNameNumber());
    }
    for (MscDocument *doc : m_documents) {
        num = std::max(num, doc->maxInstanceNameNumber());
    }
    return num;
}

int MscDocument::setInstanceNameNumbers(int nextNumber)
{
    for (MscChart *chart : m_charts) {
        nextNumber = chart->setInstanceNameNumbers(nextNumber);
    }
    for (MscDocument *doc : m_documents) {
        nextNumber = doc->setInstanceNameNumbers(nextNumber);
    }
    return nextNumber;
}

}
