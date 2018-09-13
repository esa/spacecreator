#ifndef MSCCHART_H
#define MSCCHART_H

#include <QString>
#include <QVector>

namespace msc {
class MscInstance;

class MscChart
{
public:
    MscChart(const QString &name = "");

    const QString &name() const;
    void setName(const QString &name);

    const QVector<MscInstance *> &instances() const;
    void addInstance(MscInstance *instance);

    MscInstance *instanceByName(const QString &name);

private:
    QString m_name;
    QVector<MscInstance *> m_instances;
};

} // namespace msc

#endif // MSCCHART_H
