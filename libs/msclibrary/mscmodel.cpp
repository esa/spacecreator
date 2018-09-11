#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"

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

void MscModel::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();
}

} // namespace msc
