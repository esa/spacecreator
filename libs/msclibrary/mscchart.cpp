#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"

namespace msc {

MscChart::MscChart(QObject *parent)
    : QObject(parent)
{
}

MscChart::MscChart(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
}

MscChart::~MscChart()
{
    qDeleteAll(m_instances);
    m_instances.clear();

    qDeleteAll(m_messages);
    m_messages.clear();
}

const QString &MscChart::name() const
{
    return m_name;
}

void MscChart::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    Q_EMIT nameChanged(m_name);
}

const QVector<MscInstance *> &MscChart::instances() const
{
    return m_instances;
}

void MscChart::addInstance(MscInstance *instance)
{
    if (instance == nullptr) {
        return;
    }
    if (m_instances.contains(instance)) {
        return;
    }

    m_instances.append(instance);
    Q_EMIT instanceAdded(instance);
}

MscInstance *MscChart::instanceByName(const QString &name) const
{
    for (MscInstance *instance : m_instances) {
        if (instance->name() == name) {
            return instance;
        }
    }
    return nullptr;
}

const QVector<MscMessage *> &MscChart::messages() const
{
    return m_messages;
}

void MscChart::addMessage(MscMessage *message)
{
    if (message == nullptr) {
        return;
    }
    if (m_messages.contains(message)) {
        return;
    }

    m_messages.append(message);
    Q_EMIT messageAdded(message);
}

MscMessage *MscChart::messageByName(const QString &name) const
{
    for (MscMessage *message : m_messages) {
        if (message->name() == name) {
            return message;
        }
    }
    return nullptr;
}

} // namespace msc
