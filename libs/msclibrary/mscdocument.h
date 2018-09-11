#ifndef MSCDOCUMENT_H
#define MSCDOCUMENT_H

#include <QString>
#include <QVector>

namespace msc {
class MscChart;

class MscDocument
{
public:
    explicit MscDocument(const QString &name = "");
    ~MscDocument();

    const QString &name() const;
    void setName(const QString &name);

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

private:
    QString m_name;
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
};

} // namespace msc

#endif // MSCDOCUMENT_H
