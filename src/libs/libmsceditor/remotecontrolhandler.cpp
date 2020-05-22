/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "remotecontrolhandler.h"

#include "chartviewmodel.h"
#include "commands/common/commandsstack.h"
#include "mainmodel.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "msctimer.h"
#include "mscmodel.h"

#include <QMetaEnum>
#include <QUndoStack>

/*!
 * \class RemoteControlHandler
 *
 * Handles the remote control of the view.
 */
RemoteControlHandler::RemoteControlHandler(QObject *parent)
    : QObject(parent)
{
}

void RemoteControlHandler::setModel(MainModel *model)
{
    m_model = model;
}

/*!
 * \brief RemoteControlHandler::handleRemoteCommand Perform a remote command
 * \param commandType The type of the command to perform
 * \param params Parameters for the command
 * \param peerName The caller name
 *
 * This is the main function. Call this to perform a command.
 */
void RemoteControlHandler::handleRemoteCommand(
        RemoteControlWebServer::CommandType commandType, const QVariantMap &params, const QString &peerName)
{
    if (!m_model) {
        Q_EMIT commandDone(commandType, false, peerName, QLatin1String("Empty model"));
        return;
    }
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    bool result = false;
    if (!mscChart) {
        Q_EMIT commandDone(commandType, result, peerName, QLatin1String("Empty document"));
        return;
    }

    QString errorString;
    switch (commandType) {
    case RemoteControlWebServer::CommandType::Instance:
        result = handleInstanceCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::StopInstance:
        result = handleInstanceStopCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::Message:
        result = handleMessageCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::Timer:
        result = handleTimerCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::Action:
        result = handleActionCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::Condition:
        result = handleConditionCommand(params, &errorString);
        break;
    case RemoteControlWebServer::CommandType::Undo:
        result = msc::cmd::CommandsStack::current()->canUndo();
        if (result)
            msc::cmd::CommandsStack::current()->undo();
        else
            errorString = tr("Nothing to Undo");
        break;
    case RemoteControlWebServer::CommandType::Redo:
        result = msc::cmd::CommandsStack::current()->canRedo();
        if (result)
            msc::cmd::CommandsStack::current()->redo();
        else
            errorString = tr("Nothing to Redo");
        break;
    case RemoteControlWebServer::CommandType::Save: {
        m_model->setCurrentFilePath(params.value(QLatin1String("fileName"), m_model->currentFilePath()).toString());
        msc::MscModel *mscModel = m_model->mscModel();
        mscModel->setDataLanguage(tr("ASN.1"));
        mscModel->setDataDefinitionString(params.value(QLatin1String("asn1File"), mscModel->dataDefinitionString()).toString());
        result = !m_model->currentFilePath().isEmpty();
        if (result)
            m_model->saveMsc(m_model->currentFilePath());
        else
            errorString = tr("Empty filename");
    } break;
    case RemoteControlWebServer::CommandType::VisibleItemLimit: {
        const int number = params.value(QLatin1String("number")).toInt(&result);
        if (!result)
            errorString = tr("Wrong limit number for items visibility");
        else
            m_model->chartViewModel().setVisibleItemLimit(number);
    } break;
    default:
        qWarning() << "Unknown command:" << commandType;
        errorString = tr("Unknown command");
        break;
    }
    if (result)
        m_model->chartViewModel().updateLayout();

    Q_EMIT commandDone(commandType, result, peerName, errorString);
}

/*!
 * \brief RemoteControlHandler::handleInstanceCommand Handle a remote instance command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleInstanceCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const int instanceIdx = mscChart->instances().size();
    const QString name = params.value(QLatin1String("name"), QStringLiteral("Instance_%1").arg(instanceIdx)).toString();
    if (mscChart->instanceByName(name)) {
        *errorString = tr("Chart already has instance with the name: %1").arg(name);
        return false;
    }
    const int pos = params.value(QLatin1String("pos"), -1).toInt();

    msc::MscInstance *mscInstance = new msc::MscInstance(name, mscChart);
    mscInstance->setKind(params.value(QLatin1String("kind")).toString());
    m_model->chartViewModel().currentChart()->addInstance(mscInstance);

    const QVariantList cmdParams = { QVariant::fromValue<msc::MscInstance *>(mscInstance),
        QVariant::fromValue<msc::MscChart *>(mscChart), pos };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::CreateInstance, cmdParams);
    if (!result)
        *errorString = tr("Instance is added but unavailable for Undo/Redo actions");

    return result;
}

/*!
 * \brief RemoteControlHandler::handleInstanceStopCommand Handle a remote stop command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleInstanceStopCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const QString name = params.value(QLatin1String("name")).toString();
    msc::MscInstance *mscInstance = mscChart->instanceByName(name);
    if (!mscInstance) {
        *errorString = tr("There is no Instance with the name: %1").arg(name);
        return false;
    }
    mscInstance->setExplicitStop(true);

    const QVariantList cmdParams = { QVariant::fromValue<msc::MscInstance *>(mscInstance),
        mscInstance->explicitStop() };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::StopInstance, cmdParams);
    if (!result)
        *errorString = tr("Instance is added but unavailable for Undo/Redo actions");

    return result;
}

/*!
 * \brief RemoteControlHandler::handleMessageCommand Handle a remote message command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleMessageCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const QString sourceInstanceName = params.value(QLatin1String("srcName")).toString();
    msc::MscInstance *mscSourceInstance = mscChart->instanceByName(sourceInstanceName);
    if (!mscSourceInstance && !sourceInstanceName.isEmpty()) {
        *errorString = tr("Source instance with name %1 doesn't exist").arg(sourceInstanceName);
        return false;
    }

    const QString targetInstanceName = params.value(QLatin1String("dstName")).toString();
    msc::MscInstance *mscTargetInstance = mscChart->instanceByName(targetInstanceName);
    if (!mscTargetInstance && !targetInstanceName.isEmpty()) {
        *errorString = tr("Target instance with name %1 doesn't exist").arg(targetInstanceName);
        return false;
    }

    if (!mscSourceInstance && !mscTargetInstance) {
        *errorString = tr("Can't create Message without source or target Instances");
        return false;
    }
    const QMetaEnum qtEnum = QMetaEnum::fromType<msc::MscMessage::MessageType>();
    static const int defaultValue = static_cast<int>(msc::MscMessage::MessageType::Message);
    const QString msgTypeStr = params.value(qtEnum.name(), qtEnum.valueToKey(defaultValue)).toString();
    const int msgTypeInt = qtEnum.keyToValue(msgTypeStr.toLocal8Bit().constData());
    const msc::MscMessage::MessageType messageType = static_cast<msc::MscMessage::MessageType>(msgTypeInt);

    const int pos = params.value(QLatin1String("pos"), -1).toInt();
    const int messageIdx = mscChart->instanceEvents().size();
    const QString name = params.value(QLatin1String("name"),
                                       messageType == msc::MscMessage::MessageType::Message
                                               ? QStringLiteral("Message_%1").arg(messageIdx)
                                               : QStringLiteral("Create_%1").arg(messageIdx))
                                 .toString();
    if (messageType == msc::MscMessage::MessageType::Create && !mscTargetInstance) {
        *errorString = tr("Can't create Message with type Create without target Instance");
        return false;
    }
    msc::MscMessage *message = messageType == msc::MscMessage::MessageType::Message ? new msc::MscMessage(mscChart)
                                                                                    : new msc::MscCreate(mscChart);
    message->setName(name);
    if (messageType == msc::MscMessage::MessageType::Create && mscTargetInstance)
        mscTargetInstance->setExplicitCreator(mscSourceInstance);
    message->setTargetInstance(mscTargetInstance);
    message->setSourceInstance(mscSourceInstance);
    mscChart->addInstanceEvent(message, pos);

    const QVariantList cmdParams = { QVariant::fromValue<msc::MscMessage *>(message),
        QVariant::fromValue<msc::ChartViewModel *>(&m_model->chartViewModel()), pos };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::CreateMessage, cmdParams);
    if (!result)
        *errorString = tr("Message is added but unavailable for Undo/Redo actions");

    return result;
}

/*!
 * \brief RemoteControlHandler::handleTimerCommand Handle a remote timer command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleTimerCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const QString instanceName = params.value(QLatin1String("instanceName")).toString();
    msc::MscInstance *mscInstance = mscChart->instanceByName(instanceName);
    if (!mscInstance) {
        *errorString = tr("Instance with name='%1' doesn't exist").arg(instanceName);
        return false;
    }

    const int pos = params.value(QLatin1String("pos"), -1).toInt();

    const QMetaEnum qtEnum = QMetaEnum::fromType<msc::MscTimer::TimerType>();
    static const int defaultValue = static_cast<int>(msc::MscTimer::TimerType::Unknown);
    const QString timerTypeStr = params.value(qtEnum.name(), qtEnum.valueToKey(defaultValue)).toString();
    const int timerTypeInt = qtEnum.keyToValue(timerTypeStr.toLocal8Bit().constData());
    if (timerTypeInt == defaultValue || timerTypeInt == -1) {
        *errorString = tr("Unknown Timer type");
        return false;
    }

    const int timerIdx = mscChart->instanceEvents().size();
    const msc::MscTimer::TimerType timerType = static_cast<msc::MscTimer::TimerType>(timerTypeInt);
    const QString name = params.value(QLatin1String("name"), QStringLiteral("Timer_%1").arg(timerIdx)).toString();
    msc::MscTimer *mscTimer = new msc::MscTimer(name, timerType, mscChart);
    mscTimer->setInstance(mscInstance);
    mscChart->addInstanceEvent(mscTimer, pos);

    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscTimer *>(mscTimer),
        QVariant::fromValue<msc::MscTimer::TimerType>(timerType), QVariant::fromValue<msc::MscChart *>(mscChart),
        QVariant::fromValue<msc::MscInstance *>(mscInstance), pos };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::CreateTimer, cmdParams);
    if (!result)
        *errorString = tr("Timer is added but unavailable for Undo/Redo actions");

    return result;
}

/*!
 * \brief RemoteControlHandler::handleActionCommand Handle a remote action command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleActionCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const QString instanceName = params.value(QLatin1String("instanceName")).toString();
    msc::MscInstance *mscInstance = mscChart->instanceByName(instanceName);
    if (!mscInstance) {
        *errorString = tr("Instance with name='%1' doesn't exist").arg(instanceName);
        return false;
    }

    const int pos = params.value(QLatin1String("pos"), -1).toInt();

    msc::MscAction *mscAction = new msc::MscAction(mscChart);
    const int actionIdx = mscChart->instanceEvents().size();
    const QString name = params.value(QLatin1String("name"), QStringLiteral("Action_%1").arg(actionIdx)).toString();
    mscAction->setInformalAction(name);
    mscAction->setInstance(mscInstance);
    mscChart->addInstanceEvent(mscAction, pos);

    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscAction *>(mscAction),
        QVariant::fromValue<msc::MscChart *>(mscChart), QVariant::fromValue<msc::MscInstance *>(mscInstance), pos };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::CreateAction, cmdParams);
    if (!result)
        *errorString = tr("Action is added but unavailable for Undo/Redo actions");

    return result;
}

/*!
 * \brief RemoteControlHandler::handleConditionCommand Handle a remote condition command
 * \param params
 * \param errorString
 * \return
 */
bool RemoteControlHandler::handleConditionCommand(const QVariantMap &params, QString *errorString)
{
    msc::MscChart *mscChart = m_model->chartViewModel().currentChart();
    const QString instanceName = params.value(QLatin1String("instanceName")).toString();
    msc::MscInstance *mscInstance = mscChart->instanceByName(instanceName);
    if (!mscInstance) {
        *errorString = tr("Instance with name='%1' doesn't exist").arg(instanceName);
        return false;
    }

    const int pos = params.value(QLatin1String("pos"), -1).toInt();
    const int conditionIdx = mscChart->instanceEvents().size();
    const QString name =
            params.value(QLatin1String("name"), QStringLiteral("Condition_%1").arg(conditionIdx)).toString();
    msc::MscCondition *mscCondition = new msc::MscCondition(name, mscChart);
    mscCondition->setInstance(mscInstance);
    mscCondition->setShared(params.value(QLatin1String("shared")).toBool());
    mscChart->addInstanceEvent(mscCondition, pos);

    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscCondition *>(mscCondition),
        QVariant::fromValue<msc::MscChart *>(mscChart), QVariant::fromValue<msc::MscInstance *>(mscInstance), pos };
    const bool result = msc::cmd::CommandsStack::push(msc::cmd::Id::CreateCondition, cmdParams);
    if (!result)
        *errorString = tr("Condition is added but unavailable for Undo/Redo actions");

    return result;
}
