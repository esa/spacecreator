/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "mscmessage.h"

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "mscinstance.h"

#include <QDebug>

/*!
   \class msc::MscMessage
   \brief Class to describe a MSC message
 */

namespace msc {

const cif::CifLine::CifType MscMessage::m_messageCifMainType = cif::CifLine::CifType::Message;

MscMessage::MscMessage(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscMessage::MscMessage(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

/*!
   \fn MscMessage::messageInstanceName
   In case, where the \<message name\> and the \<output address\> or \<input address\>
   are not sufficient for a unique mapping the \<message instance name\> has to be employed.
   \sa msc::MscEntity::name() sourceInstance() targetInstance()
 */
const QString &MscMessage::messageInstanceName() const
{
    return m_messageInstanceName;
}

void MscMessage::setMessageInstanceName(const QString &name)
{
    if (name == m_messageInstanceName) {
        return;
    }

    m_messageInstanceName = name;
    Q_EMIT messageInstanceNameChanged();
    Q_EMIT dataChanged();
}

/*!
   \fn MscMessage::fullName
   \return The full name of the message including name and message instance name
 */
QString MscMessage::fullName() const
{
    return m_messageInstanceName.isEmpty() ? name() : QString("%1,%2").arg(name(), m_messageInstanceName);
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
    Q_EMIT dataChanged();
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
    Q_EMIT dataChanged();
}

MscEntity::EntityType MscMessage::entityType() const
{
    return MscEntity::EntityType::Message;
}

const MscParameterList &MscMessage::parameters() const
{
    return m_parameters;
}

void MscMessage::setParameters(const MscParameterList &parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;

    Q_EMIT parameterStringChanged();
    Q_EMIT dataChanged();
}

/*!
   Returns the the given message is defining the same.
   As per spec the identification is done by name, message instance name, parameter, in- and out instance
 */
bool MscMessage::isSame(const MscMessage *message) const
{
    bool same =
            (fullName() == message->fullName()) && (m_source == message->m_source) && (m_target == message->m_target);
    if (!same)
        return false;

    const int parametersSize = m_parameters.size();
    if (parametersSize != message->m_parameters.size())
        return false;

    for (int i = 0; i < parametersSize; ++i) {
        if (m_parameters.at(i) != message->m_parameters.at(i))
            return false;
    }

    return true;
}

MscMessage::MessageType MscMessage::messageType() const
{
    return m_msgType;
}

bool MscMessage::relatesTo(const MscInstance *instance) const
{
    return m_source == instance || m_target == instance;
}

/*!
   \brief MscMessage::isGlobal
   \return true if the message is a message to/from Env, gate, etc (has only one instance set)
 */
bool MscMessage::isGlobal() const
{
    const bool isRegular = messageType() == MscMessage::MessageType::Message;
    const bool onlySource = m_source && !m_target;
    const bool onlyTarget = !m_source && m_target;
    return isRegular && (onlySource || onlyTarget);
}

/*!
   \brief MscMessage::isOrphan
   \return true if the message is not connected to any instance
 */
bool MscMessage::isOrphan() const
{
    return !(m_source || m_target);
}

/*!
   Returns if the message is already used by an in and an out
 */
bool MscMessage::isConnected() const
{
    return m_descrIn.to != nullptr && m_descrOut.from != nullptr;
}

QString MscMessage::toDbgString() const
{
    auto instanceName = [](MscInstance *instance) {
        static const QString envName("ENV");
        return instance ? instance->name() : envName;
    };
    return QString("%1->%2:%3").arg(instanceName(m_source), instanceName(m_target), name());
}

cif::CifBlockShared MscMessage::cifMessage() const
{
    return cifBlockByType(m_messageCifMainType);
}

QVector<QPoint> MscMessage::cifPoints() const
{
    using namespace cif;

    if (const cif::CifBlockShared &cif = cifMessage())
        if (cif->hasPayloadFor(m_messageCifMainType))
            return cif->payload(m_messageCifMainType).value<QVector<QPoint>>();

    return {};
}

void MscMessage::setCifPoints(const QVector<QPoint> &points)
{
    using namespace cif;

    if (points.isEmpty()) {
        clearCifs();
        Q_EMIT cifPointsChanged();
        Q_EMIT dataChanged(); // update MscTextView
        return;
    }

    CifBlockShared msgCif = cifMessage();
    if (!msgCif) {
        msgCif = CifBlockFactory::createBlockMessage();
        addCif(msgCif);
    }

    if (!msgCif->hasPayloadFor(m_messageCifMainType)) {
        msgCif->addLine(CifLineShared(new CifLineMessage()));
    }

    msgCif->setPayload(QVariant::fromValue(points), m_messageCifMainType);
    Q_EMIT cifPointsChanged();
    Q_EMIT dataChanged(); // update MscTextView
}

QString MscMessage::paramString() const
{
    QString parameters;
    for (const MscParameter &param : this->parameters()) {
        if (!parameters.isEmpty())
            parameters += ", ";
        parameters += param.pattern().isEmpty() ? param.expression() : param.pattern();
    }
    return parameters;
}

} // namespace msc

QDebug operator<<(QDebug dbg, const msc::MscMessage &message)
{
    QDebugStateSaver saver(dbg);
    dbg << message.toDbgString();
    return dbg;
}
