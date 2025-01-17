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

#include "mscmodel.h"

#include "asn1systemchecks.h"
#include "cifblockfactory.h"
#include "ciflines.h"
#include "errorhub.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"

#include <QDebug>
#include <algorithm>

namespace msc {

MscModel::MscModel(QObject *parent)
    : shared::DataModel(parent)
{
}

MscModel::~MscModel()
{
    clear();
}

std::unique_ptr<MscModel> MscModel::defaultModel()
{
    auto model = std::make_unique<MscModel>();
    auto doc = new MscDocument(tr("Untitled_Document"));
    doc->setHierarchyType(MscDocument::HierarchyAnd);

    // leaf type document
    auto leafDoc = new MscDocument(tr("Untitled_Leaf"));
    leafDoc->setHierarchyType(MscDocument::HierarchyLeaf);
    leafDoc->addChart(new MscChart(tr("Untitled_MSC")));

    doc->addDocument(leafDoc);
    model->addDocument(doc);

    return model;
}

conversion::ModelType MscModel::modelType() const
{
    return conversion::ModelProperties<MscModel>::type;
}

void MscModel::setFilename(const QString &filename)
{
    if (filename == m_filename) {
        return;
    }

    m_filename = filename;
    Q_EMIT filenameChanged();
}

const QString MscModel::filename() const
{
    return m_filename;
}

const QVector<MscDocument *> &MscModel::documents() const
{
    return m_documents;
}

bool MscModel::addDocument(MscDocument *document)
{
    if (document == nullptr) {
        return false;
    }
    if (m_documents.contains(document)) {
        return false;
    }

    document->setParent(this);
    m_documents.append(document);
    connect(document, &MscDocument::dataChanged, this, &MscModel::dataChanged);
    connect(document, &MscDocument::documentRemovedFrom, this, &MscModel::documentRemovedFrom);
    Q_EMIT documentAdded(document);
    Q_EMIT dataChanged();

    return true;
}

void MscModel::removeDocument(MscDocument *document)
{
    if (document == nullptr) {
        return;
    }
    if (!m_documents.contains(document)) {
        return;
    }

    if (m_documents.removeAll(document)) {
        document->clear();
    }

    Q_EMIT documentRemovedFrom(document, this);
}

/*!
   Returns the charts owned by this model directly
 */
const QVector<MscChart *> &MscModel::charts() const
{
    return m_charts;
}

void MscModel::addChart(MscChart *chart)
{
    if (chart == nullptr) {
        return;
    }
    if (m_charts.contains(chart)) {
        return;
    }

    chart->setParent(this);
    m_charts.append(chart);
    connect(chart, &MscChart::dataChanged, this, &MscModel::dataChanged);
    Q_EMIT chartAdded(chart);
    Q_EMIT dataChanged();
}

/*!
 * \brief MainModel::firstChart Get the first chart
 * \return First chart pointer or null
 */
MscChart *MscModel::firstChart() const
{
    if (!m_charts.empty()) {
        return m_charts.at(0);
    }

    return firstChart(m_documents);
}

/*!
 * Returns the first document. If there is no document, it returns the first chart;
 */
MscEntity *MscModel::firstEntity() const
{
    if (!m_documents.isEmpty()) {
        return m_documents[0];
    }

    if (!m_charts.isEmpty()) {
        return m_charts[0];
    }

    return nullptr;
}

void addChildDocuments(msc::MscDocument *doc, QVector<MscDocument *> &allDocs)
{
    allDocs += doc->documents();
    for (msc::MscDocument *child : doc->documents()) {
        addChildDocuments(child, allDocs);
    }
}

/*!
   Returns all documents of this model in one flat list.
 */
QVector<MscDocument *> MscModel::allDocuments() const
{
    QVector<MscDocument *> documents = m_documents;
    for (msc::MscDocument *child : m_documents) {
        addChildDocuments(child, documents);
    }
    return documents;
}

/*!
   Returns all charts of the whole model in one flat list. So this includes all charts owned by documents
 */
QVector<MscChart *> MscModel::allCharts() const
{
    QVector<MscChart *> charts = m_charts;
    for (msc::MscDocument *doc : allDocuments()) {
        charts += doc->charts();
    }
    return charts;
}

const QString &MscModel::dataLanguage() const
{
    return m_dataLanguage;
}

void MscModel::setDataLanguage(const QString &language)
{
    if (language == m_dataLanguage) {
        return;
    }

    m_dataLanguage = language;
    Q_EMIT dataLanguageChanged(m_dataLanguage);
    Q_EMIT dataChanged();
}

const QString &MscModel::dataDefinitionString() const
{
    return m_dataDefinitionString;
}

void MscModel::setDataDefinitionString(const QString &dataString)
{
    if (dataString == m_dataDefinitionString) {
        return;
    }

    m_dataDefinitionString = dataString;

    Q_EMIT dataDefinitionStringChanged(m_dataDefinitionString);
    Q_EMIT dataChanged();
}

void msc::MscModel::checkAllMessages()
{
    shared::ErrorHub::clearFileErrors(m_filename);

    QStringList faultyMessages;
    const bool ok = checkAllMessagesForAsn1Compliance(&faultyMessages);
    if (!ok) {
        for (const QString &error : faultyMessages) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Message ASN.1 error: ").arg(error), m_filename);
        }
    }
}

void MscModel::setAsn1Checks(Asn1Acn::Asn1SystemChecks *checks)
{
    m_asnChecks = checks;
}

Asn1Acn::Asn1SystemChecks *MscModel::asn1Checks() const
{
    return m_asnChecks;
}

void MscModel::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    setDataLanguage("");

    Q_EMIT cleared();
    Q_EMIT dataChanged();
}

/*!
   Goes through all messages in the model and checks if there are "duplicate" messages
   that need to have the message instance name set.
   The instance name used is a simple number/counter, that starts at 1, or the
   highest existing number (+1) in any instance name. That counter
   is used for the whole model, and not per instance or chart.
   Existing instance names won't be overwritten.

   \see MscChart::setInstanceNameNumbers
 */
void MscModel::checkInstanceNames()
{
    int nextNumber = 1;
    for (const MscChart *chart : m_charts) {
        nextNumber = std::max(nextNumber, chart->maxInstanceNameNumber());
    }
    for (const MscDocument *doc : m_documents) {
        nextNumber = std::max(nextNumber, doc->maxInstanceNameNumber());
    }

    for (MscChart *chart : m_charts) {
        nextNumber = chart->setInstanceNameNumbers(nextNumber);
    }
    for (MscDocument *doc : m_documents) {
        nextNumber = doc->setInstanceNameNumbers(nextNumber);
    }
}

/*!
   Checks if the parameters of the \p message comply to the ASN1 definition
   The message name has to match a \see msc::MscMessageDeclaration
 */
bool MscModel::checkMessageAsn1Compliance(const msc::MscMessage &message) const
{
    if (m_documents.isEmpty()) {
        return false;
    }

    const msc::MscMessageDeclarationList *messageDeclarations = m_documents.at(0)->messageDeclarations();
    if (!messageDeclarations) {
        return false;
    }

    msc::MscMessageDeclaration *usedDeclaration = nullptr;
    for (msc::MscMessageDeclaration *declaration : *messageDeclarations) {
        if (declaration && declaration->names().contains(message.name())) {
            usedDeclaration = declaration;
        }
    }

    if (usedDeclaration) {
        if (message.parameters().size() != usedDeclaration->typeRefList().size()) {
            return false;
        }
        for (int i = 0; i < message.parameters().size(); ++i) {
            const QString &parameter = message.parameters().at(i).parameter();
            const QString &type = usedDeclaration->typeRefList().at(i);
            if (m_asnChecks && !m_asnChecks->checkAsn1Compliance(parameter, type)) {
                return false;
            }
        }
        // All parameters comply
        return true;
    }

    // no matching declaration available
    return true;
}

/*!
   Checks all messages in this model if it complies the asn1 definition
 */
bool MscModel::checkAllMessagesForAsn1Compliance(QStringList *faultyMessages) const
{
    QVector<msc::MscMessage *> messages;

    for (msc::MscDocument *childDoc : m_documents) {
        appendMessages(childDoc, messages);
    }
    for (msc::MscChart *childChart : m_charts) {
        messages.append(childChart->messages());
    }

    bool ok = true;

    for (msc::MscMessage *message : messages) {
        if (!checkMessageAsn1Compliance(*message)) {
            ok = false;
            if (faultyMessages) {
                *faultyMessages << QString("%1(%2)").arg(message->name(), message->paramString());
            }
        }
    }

    return ok;
}

/*!
 * Add informations like URL for requrements or reviews
 */
void MscModel::addDocumentMetaDataToFirstEntity()
{
    MscEntity *entity = firstEntity();
    if (!entity) {
        return;
    }

    removeDocumentMetaDataFromFirstEntity();

    if (requirementsURL().isValid()) {
        // If the requirements url block is there already, update it
        QVector<cif::CifBlockShared> cifs = entity->cifs();
        for (const cif::CifBlockShared &block : cifs) {
            if (block->blockType() == cif::CifLine::CifType::RequirementsUrl) {
                block->setPayload(requirementsURL(), cif::CifLine::CifType::RequirementsUrl);
                return;
            }
        }

        // no requirements url block there yet, so add it
        cif::CifBlockShared requirementsUrlCif =
                cif::CifBlockFactory::createBlock({ cif::CifLineShared(new cif::CifLineRequirementsUrl()) });
        requirementsUrlCif->setPayload(QVariant::fromValue(requirementsURL()), cif::CifLine::CifType::RequirementsUrl);
        cifs.append(requirementsUrlCif);
        entity->setCifs(cifs);
    }

    if (reviewsURL().isValid()) {
        // If the reviews url block is there already, update it
        QVector<cif::CifBlockShared> cifs = entity->cifs();
        for (const cif::CifBlockShared &block : cifs) {
            if (block->blockType() == cif::CifLine::CifType::ReviewsUrl) {
                block->setPayload(reviewsURL(), cif::CifLine::CifType::ReviewsUrl);
                return;
            }
        }

        // no reviews url block there yet, so add it
        cif::CifBlockShared reviewsUrlCif =
                cif::CifBlockFactory::createBlock({ cif::CifLineShared(new cif::CifLineReviewsUrl()) });
        reviewsUrlCif->setPayload(QVariant::fromValue(reviewsURL()), cif::CifLine::CifType::ReviewsUrl);
        cifs.append(reviewsUrlCif);
        entity->setCifs(cifs);
    }
}

/*!
 * Removes informations like URL for requrements or reviews
 */
void MscModel::removeDocumentMetaDataFromFirstEntity()
{
    MscEntity *entity = firstEntity();
    if (!entity) {
        return;
    }

    QVector<cif::CifBlockShared> cifs = entity->cifs();
    auto it = std::remove_if(cifs.begin(), cifs.end(), [](const cif::CifBlockShared &block) {
        return (block->blockType() == cif::CifLine::CifType::RequirementsUrl)
                || (block->blockType() == cif::CifLine::CifType::ReviewsUrl);
    });
    cifs.erase(it, cifs.end());

    entity->setCifs(cifs);
}

void MscModel::appendMessages(msc::MscDocument *doc, QVector<msc::MscMessage *> &messages) const
{
    for (msc::MscDocument *childDoc : doc->documents()) {
        appendMessages(childDoc, messages);
    }
    for (msc::MscChart *childChart : doc->charts()) {
        messages.append(childChart->messages());
    }
}

/*!
 * \brief MainModel::firstChart Get the first chart of the \a docs
 * \return The first chart or null
 */
MscChart *MscModel::firstChart(const QVector<MscDocument *> &docs) const
{
    for (MscDocument *doc : docs) {
        if (!doc->charts().isEmpty()) {
            return doc->charts().at(0);
        }
        auto ret = firstChart(doc->documents());
        if (ret != nullptr) {
            return ret;
        }
    }
    return nullptr;
}

} // namespace msc
