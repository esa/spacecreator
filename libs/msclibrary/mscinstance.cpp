#include "mscinstance.h"

namespace msc {

MscInstance::MscInstance(QObject *parent)
    : QObject(parent)
{
}

MscInstance::MscInstance(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

const QString &MscInstance::name() const
{
    return m_name;
}

void MscInstance::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

const QString &MscInstance::kind() const
{
    return m_kind;
}

void MscInstance::setKind(const QString &kind)
{
    if (kind == m_kind) {
        return;
    }

    m_kind = kind;
    Q_EMIT kindChanged(m_kind);
}

const QStringList &MscInstance::decomposition() const
{
    return m_decomposition;
}

void MscInstance::setDecomposition(const QStringList &decomposition)
{
    if (decomposition == m_decomposition) {
        return;
    }

    m_decomposition = decomposition;
    Q_EMIT decompositionChanged(m_decomposition);
}

} // namespace msc
