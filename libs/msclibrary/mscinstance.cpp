#include "mscinstance.h"

namespace msc {

MscInstance::MscInstance(const QString &name)
    : m_name(name)
{
}

const QString &MscInstance::name() const
{
    return m_name;
}

void MscInstance::setName(const QString &name)
{
    m_name = name;
}

} // namespace msc
