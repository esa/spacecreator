#include "mscchart.h"
#include "mscinstance.h"

namespace msc {

MscChart::MscChart(const QString &name)
    : m_name(name)
{
}

const QString &MscChart::name() const
{
    return m_name;
}

void MscChart::setName(const QString &name)
{
    m_name = name;
}

const QVector<MscInstance *> &MscChart::instances() const
{
    return m_instances;
}

void MscChart::addInstance(MscInstance *instance)
{
    m_instances.append(instance);
}

MscInstance *MscChart::instanceByName(const QString &name)
{
    for (MscInstance *instance : m_instances) {
        if (instance->name() == name) {
            return instance;
        }
    }
    return nullptr;
}

} // namespace msc
