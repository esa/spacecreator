#include "mscdocument.h"
#include "mscchart.h"

namespace msc {

MscDocument::MscDocument(QObject *parent)
    : QObject(parent)
{
}

MscDocument::MscDocument(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
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
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

const QVector<MscDocument *> &MscDocument::documents() const
{
    return m_documents;
}

void MscDocument::addDocument(MscDocument *document)
{
    m_documents.append(document);
    Q_EMIT documentAdded(document);
}

const QVector<MscChart *> &MscDocument::charts() const
{
    return m_charts;
}

void MscDocument::addChart(MscChart *chart)
{
    m_charts.append(chart);
    Q_EMIT chartAdded(chart);
}

void MscDocument::clear()
{
    m_name.clear();

    qDeleteAll(m_documents);
    m_documents.clear();

    qDeleteAll(m_charts);
    m_charts.clear();

    Q_EMIT cleared();
}

} // namespace msc
