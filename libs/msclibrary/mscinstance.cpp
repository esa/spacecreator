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

const QString &MscInstance::kind() const
{
    return m_kind;
}

void MscInstance::setKind(const QString &kind)
{
    m_kind = kind;
}

const QStringList &MscInstance::decomposition() const
{
    return m_decomposition;
}

void MscInstance::setDecomposition(const QStringList &decomposition)
{
    m_decomposition = decomposition;
}

} // namespace msc
