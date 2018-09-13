#include "mscmessage.h"

namespace msc {

MscMessage::MscMessage(const QString &name)
    : m_name(name)
{
}

const QString &MscMessage::name() const
{
    return m_name;
}

void MscMessage::setName(const QString &name)
{
    m_name = name;
}

MscInstance *MscMessage::sourceInstance() const
{
    return m_source;
}

void MscMessage::setSourceInstance(MscInstance *source)
{
    m_source = source;
}

MscInstance *MscMessage::targetInstance() const
{
    return m_target;
}

void MscMessage::setTargetInstance(MscInstance *target)
{
    m_target = target;
}

} // namespace msc
