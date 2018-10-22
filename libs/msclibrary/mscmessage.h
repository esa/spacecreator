#ifndef MSCMESSAGE_H
#define MSCMESSAGE_H

#include <QObject>
#include <QString>

namespace msc {
class MscInstance;

class MscMessage : public QObject
{
    Q_OBJECT
public:
    explicit MscMessage(QObject *parent = nullptr);
    MscMessage(const QString &name, QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &name);

    MscInstance *sourceInstance() const;
    void setSourceInstance(MscInstance *source);

    MscInstance *targetInstance() const;
    void setTargetInstance(MscInstance *target);

Q_SIGNALS:
    void nameChanged(const QString &name);
    void sourceChanged(MscInstance *source);
    void targetChanged(MscInstance *target);

private:
    QString m_name;
    MscInstance *m_source = nullptr;
    MscInstance *m_target = nullptr;
};

} // namespace msc

#endif // MSCMESSAGE_H
