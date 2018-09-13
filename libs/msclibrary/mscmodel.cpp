#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmessage.h"

namespace msc {

MscModel::MscModel()
{
}

MscModel::~MscModel()
{
    clear();
}

const QVector<MscDocument *> &MscModel::documents() const
{
    return m_documents;
}

void MscModel::addDocument(MscDocument *document)
{
    m_documents.append(document);
}

const QVector<MscChart *> &MscModel::charts() const
{
    return m_charts;
}

void MscModel::addChart(MscChart *chart)
{
    m_charts.append(chart);
}

const QVector<MscMessage *> &MscModel::messages() const
{
    return m_messages;
}

void MscModel::addMessage(MscMessage *message)
{
    m_messages.append(message);
}

void MscModel::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    qDeleteAll(m_messages);
    m_messages.clear();
}

} // namespace msc
