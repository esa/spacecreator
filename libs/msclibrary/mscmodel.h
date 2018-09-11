#ifndef MODEL_H
#define MODEL_H

#include <QVector>

namespace msc {
class MscChart;
class MscDocument;

class MscModel
{
public:
    MscModel();
    ~MscModel();

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
};

} // namespace msc

#endif // MODEL_H
