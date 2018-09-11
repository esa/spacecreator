#include "mscdocument.h"
#include "mscchart.h"

namespace msc {

MscDocument::MscDocument(const QString &name)
    : m_name(name)
{
}

MscDocument::~MscDocument()
{
    clear();
}

const QString &MscDocument::name() const
{
    return m_name;
}

void MscDocument::setName(const QString &name)
{
    m_name = name;
}

const QVector<MscDocument *> &MscDocument::documents() const
{
    return m_documents;
}

void MscDocument::addDocument(MscDocument *document)
{
    m_documents.append(document);
}

const QVector<MscChart *> &MscDocument::charts() const
{
    return m_charts;
}

void MscDocument::addChart(MscChart *chart)
{
    m_charts.append(chart);
}

void MscDocument::clear()
{
    m_name.clear();

    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();
}

} // namespace msc
