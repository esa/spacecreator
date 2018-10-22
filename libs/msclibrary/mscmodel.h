#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>

namespace msc {
class MscChart;
class MscDocument;

class MscModel : public QObject
{
    Q_OBJECT
public:
    explicit MscModel(QObject *parent = nullptr);
    ~MscModel();

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

Q_SIGNALS:
    void documentAdded(MscDocument *document);
    void chartAdded(MscChart *chart);
    void cleared();

private:
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
};

} // namespace msc

#endif // MODEL_H
