#ifndef MSCMESSAGE_H
#define MSCMESSAGE_H

#include <QString>

namespace msc {
class MscInstance;

class MscMessage
{
public:
    explicit MscMessage(const QString &name = "");

    const QString &name() const;
    void setName(const QString &name);

    MscInstance *sourceInstance() const;
    void setSourceInstance(MscInstance *source);

    MscInstance *targetInstance() const;
    void setTargetInstance(MscInstance *target);

private:
    QString m_name;
    MscInstance *m_source = nullptr;
    MscInstance *m_target = nullptr;
};

} // namespace msc

#endif // MSCMESSAGE_H
