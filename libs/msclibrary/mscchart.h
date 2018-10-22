#ifndef MSCCHART_H
#define MSCCHART_H

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {
class MscInstance;
class MscMessage;

class MscChart : public QObject
{
    Q_OBJECT
public:
    explicit MscChart(QObject *parent = nullptr);
    MscChart(const QString &name, QObject *parent = nullptr);
    ~MscChart();

    const QString &name() const;
    void setName(const QString &name);

    const QVector<MscInstance *> &instances() const;
    void addInstance(MscInstance *instance);

    MscInstance *instanceByName(const QString &name) const;

    const QVector<MscMessage *> &messages() const;
    void addMessage(MscMessage *message);

    MscMessage *messageByName(const QString &name) const;

Q_SIGNALS:
    void nameChanged(const QString &name);
    void instanceAdded(MscInstance *instance);
    void messageAdded(MscMessage *message);

private:
    QString m_name;
    QVector<MscInstance *> m_instances;
    QVector<MscMessage *> m_messages;
};

} // namespace msc

#endif // MSCCHART_H
