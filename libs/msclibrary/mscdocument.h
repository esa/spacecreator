#ifndef MSCDOCUMENT_H
#define MSCDOCUMENT_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscChart;

class MscDocument : public QObject
{
    Q_OBJECT
public:
    explicit MscDocument(QObject *parent = nullptr);
    MscDocument(const QString &name, QObject *parent = nullptr);
    ~MscDocument();

    const QString &name() const;
    void setName(const QString &name);

    const QVector<MscDocument *> &documents() const;
    void addDocument(MscDocument *document);

    const QVector<MscChart *> &charts() const;
    void addChart(MscChart *chart);

    void clear();

Q_SIGNALS:
    void nameChanged(const QString &name);
    void documentAdded(MscDocument *document);
    void chartAdded(MscChart *chart);
    void cleared();

private:
    QString m_name;
    QVector<MscDocument *> m_documents;
    QVector<MscChart *> m_charts;
};

} // namespace msc

#endif // MSCDOCUMENT_H
