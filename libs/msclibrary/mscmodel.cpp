#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"

namespace msc {

MscModel::MscModel(QObject *parent)
    : QObject(parent)
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
    if (document == nullptr) {
        return;
    }
    if (m_documents.contains(document)) {
        return;
    }

    m_documents.append(document);
    Q_EMIT documentAdded(document);
}

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

    m_charts.append(chart);
    Q_EMIT chartAdded(chart);
}

void MscModel::clear()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    Q_EMIT cleared();
}

} // namespace msc
