#include "mscmessage.h"

namespace msc {

MscMessage::MscMessage(QObject *parent)
    : QObject(parent)
{
}

MscMessage::MscMessage(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

const QString &MscMessage::name() const
{
    return m_name;
}

void MscMessage::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

MscInstance *MscMessage::sourceInstance() const
{
    return m_source;
}

void MscMessage::setSourceInstance(MscInstance *source)
{
    if (source == m_source) {
        return;
    }

    m_source = source;
    Q_EMIT sourceChanged(m_source);
}

MscInstance *MscMessage::targetInstance() const
{
    return m_target;
}

void MscMessage::setTargetInstance(MscInstance *target)
{
    if (target == m_target) {
        return;
    }

    m_target = target;
    Q_EMIT targetChanged(m_target);
}

} // namespace msc
