/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscsystemchecks.h"

#include "chartlayoutmanager.h"
#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdifaceattrchange.h"
#include "commandsstack.h"
#include "dvappmodel.h"
#include "dveditorcore.h"
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "interfacedocument.h"
#include "ivconnection.h"
#include "ivconnectionchain.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivsystemchecks.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "spacecreatorproject.h"
#include "undocommand.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QUndoStack>

namespace scs {

MscSystemChecks::MscSystemChecks(QObject *parent)
    : QObject(parent)
{
}

void MscSystemChecks::setStorage(SpaceCreatorProject *storage)
{
    m_storage = storage;

    connect(m_storage, &scs::SpaceCreatorProject::mscCoreAdded, this, [=](MSCEditorCorePtr core) {
        connect(core.data(), &msc::MSCEditorCore::nameChanged, this, &scs::MscSystemChecks::onMscEntityNameChanged);
    });

    connect(m_storage, &scs::SpaceCreatorProject::ivCoreAdded, this, [this](IVEditorCorePtr ivCore) {
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::nameChanged, this,
                &scs::MscSystemChecks::onEntityNameChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::entitiesRemoved, this,
                &scs::MscSystemChecks::onEntitiesRemoved);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::implementationChanged, this,
                &scs::MscSystemChecks::onImplementationChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::defaultImplementationChanged, this,
                &scs::MscSystemChecks::onDefaultImplementationChanged);
    });
}

/*!
   Returns if at least one instance in one of the .msc files, or if any function in any dv.xml files  has the name \p
   name
 */
bool MscSystemChecks::ivFunctionUsed(const QString &name)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscInstance *instance : chart->instances()) {
                if (instance->name() == name) {
                    return true;
                }
            }
        }
    }

    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvm::DVModel *model = dvCore->appModel()->objectsModel();
        for (dvm::DVFunction *fn : model->allObjectsByType<dvm::DVFunction>()) {
            if (fn->title() == name) {
                return true;
            }
        }
    }

    return false;
}

/*!
   Changes all instances that have the name \p oldName to have the new name \p name
 */
void MscSystemChecks::changeMscInstanceName(const QString &oldName, const QString &name)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->changeMscInstanceName(oldName, name);
    }
}

/*!
   Removes all instance that are corresponding to the function \p ivFunction
 */
void MscSystemChecks::removeMscInstances(ivm::IVFunction *ivFunction)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->removeMscInstances(ivFunction);
    }
}

/*!
   Returns if there is at least one msc instance corresponding to \p ivFunction
 */
bool MscSystemChecks::hasCorrespondingInstances(ivm::IVFunction *ivFunction) const
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        if (!mscCore->correspondingInstances(ivFunction).isEmpty()) {
            return true;
        }
    }
    return false;
}

/*!
  Returns if at least one message in one of the .msc files has the name \p messageName
 */
bool MscSystemChecks::mscMessagesExist(const QString &messageName, const QString &sourceName, const QString &targetName)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscMessage *message : chart->messages()) {
                if (message->name() == messageName) {
                    const QString messageSource = message->sourceInstance() ? message->sourceInstance()->name() : "";
                    const QString messageTarget = message->targetInstance() ? message->targetInstance()->name() : "";
                    if (messageSource == sourceName && messageTarget == targetName) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*!
   Changes all messages that have the name \p oldName to have the new name \p newName, if the source and target have the
   names \p sourceName and \p targetName
 */
void MscSystemChecks::changeMscMessageName(
        const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->changeMscMessageName(oldName, newName, sourceName, targetName);
    }
}

/*!
   Removes all messages that are corresponding to the connection \p ivConnection
 */
void MscSystemChecks::removeMscMessages(ivm::IVConnection *ivConnection)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->removeMscMessages(ivConnection);
    }
}

/*!
   Returns if there is at least one msc message corresponding to \p ivConnection
 */
bool MscSystemChecks::hasCorrespondingMessages(ivm::IVConnection *ivConnection) const
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        if (!mscCore->correspondingMessages(ivConnection).isEmpty()) {
            return true;
        }
    }
    return false;
}

/*!
   Checks if all instances have corresponding functions in the iv model
   Errors are reported in a message box
 */
void MscSystemChecks::checkInstances()
{
    QVector<MSCEditorCorePtr> mscCores = m_storage->allMscCores();

    // Check for names
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultNames;
    for (MSCEditorCorePtr &mplugin : mscCores) {
        resultNames += mplugin->systemChecker()->checkInstanceNames();
    }

    // Check for nested functions usage
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultRelations;
    for (MSCEditorCorePtr &mplugin : mscCores) {
        resultRelations += mplugin->systemChecker()->checkInstanceRelations();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following instances have no corresponding iv function:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (!resultRelations.isEmpty()) {
        if (!text.isEmpty()) {
            text += "\n\n";
        }
        text += tr("Following instances are used with parent/child of nested functions:\n");
        for (auto item : resultRelations) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }

    if (resultNames.isEmpty() && resultRelations.isEmpty()) {
        QMessageBox::information(nullptr, tr("All instances are ok"), tr("All instances are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming instances"), text);
    }
}

/*!
   Checks if all messages have corresponding connections in the iv model
   Errors are reported in a message box
 */
void MscSystemChecks::checkMessages()
{
    QVector<MSCEditorCorePtr> mscCores = m_storage->allMscCores();

    // check messages
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> resultNames;
    for (const MSCEditorCorePtr &mplugin : mscCores) {
        resultNames += mplugin->systemChecker()->checkMessages();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following messages have no corresponding iv connection:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (resultNames.isEmpty()) {
        QMessageBox::information(nullptr, tr("All messages are ok"), tr("All messages are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming messages"), text);
    }
}

/*!
   Returns if any message binding exists, that has the given source function, target function, and name for the
   side/end of the message
 */
bool MscSystemChecks::dvMessagesExist(
        const QString &messageName, const QString &sourceName, const QString &targetName, shared::MessageEnd msgSide)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvm::DVModel *model = dvCore->appModel()->objectsModel();
        for (const dvm::DVMessage *msg : model->allObjectsByType<dvm::DVMessage>()) {
            if (msg->fromFunction() == sourceName && msg->toFunction() == targetName) {
                if ((msgSide == shared::SOURCE || msgSide == shared::BOTH) && msg->fromInterface() == messageName) {
                    return true;
                }
                if ((msgSide == shared::TARGET || msgSide == shared::BOTH) && msg->toInterface() == messageName) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*!
   Changes all function bindings that have the name \p oldName to have the new name \p name
 */
void MscSystemChecks::changeDvFunctionBindingName(const QString &oldName, const QString &name)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDvFunctionBindingName(oldName, name);
    }
}

/*!
   Removes all function bindings that are corresponding to the function \p ivFunction
 */
void MscSystemChecks::removeDvFunctionBinding(ivm::IVFunction *ivFunction)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->removeDvFunctionBinding(ivFunction->title());
    }
}

/*!
   Update all DV message bindings interface name, that match the name, source/target function and message side/end
 */
void MscSystemChecks::changeDvMessageBindingName(const QString &oldName, const QString &name, const QString &sourceName,
        const QString &targetName, shared::MessageEnd msgSide)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDvMessageBindingIfName(oldName, name, sourceName, targetName, msgSide);
    }
}

/*!
   Removes all message bindings that are corresponding to the function \p ivFunction
 */
void MscSystemChecks::removeDvMessageBinding(ivm::IVConnection *ivConnection)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->removeDvMessageBinding(ivConnection->sourceName(), ivConnection->sourceInterfaceName(),
                ivConnection->targetName(), ivConnection->targetInterfaceName());
    }
}

void MscSystemChecks::onEntityNameChanged(ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command)
{
    if (m_nameUpdateRunning) {
        return;
    }

    auto cmdAttribChange = dynamic_cast<ive::cmd::CmdFunctionAttrChange *>(command);
    if (cmdAttribChange) {
        if (ivFunctionUsed(oldName)) {
            if (command->isFirstChange()) {
                const int result = QMessageBox::question(
                        nullptr, tr("Update instances"), tr("Do you want to update MSC instances / DV bindings?"));
                if (result == QMessageBox::Yes) {
                    m_nameUpdateRunning = true;
                    changeMscInstanceName(oldName, entity->title());
                    changeDvFunctionBindingName(oldName, entity->title());
                    m_nameUpdateRunning = false;
                }
            } else {
                changeMscInstanceName(oldName, entity->title());
                changeDvFunctionBindingName(oldName, entity->title());
            }
        }
    }

    auto cmdIfaceAttribChange = dynamic_cast<ive::cmd::CmdIfaceAttrChange *>(command);
    if (cmdIfaceAttribChange) {
        ivm::IVInterface *interface = cmdIfaceAttribChange->interface();
        QList<QStringList> messagesData;
        shared::MessageEnd msgEnd = shared::TARGET;
        if (interface->direction() == ivm::IVInterface::InterfaceType::Provided) {
            // Update from connections
            QVector<ivm::IVConnection *> connections = cmdIfaceAttribChange->getRelatedConnections();
            for (const ivm::IVConnection *connection : qAsConst(connections)) {
                if (mscMessagesExist(oldName, connection->sourceName(), connection->targetName())
                        || dvMessagesExist(oldName, connection->sourceName(), connection->targetName(), msgEnd)) {
                    messagesData << QStringList({ connection->sourceName(), connection->targetName() });
                }
            }
            // Update from cyclic interfaces
            ivm::IVFunctionType *func = interface->function();
            if (func && interface->kind() == ivm::IVInterface::OperationKind::Cyclic) {
                if (mscMessagesExist(oldName, func->title(), "")) {
                    messagesData << QStringList({ func->title(), "" });
                }
                if (mscMessagesExist(oldName, "", func->title())) {
                    messagesData << QStringList({ "", func->title() });
                }
            }
        } else {
            msgEnd = shared::SOURCE;
            QVector<ivm::IVConnection *> connections = cmdIfaceAttribChange->getRelatedConnections();
            for (const ivm::IVConnection *connection : qAsConst(connections)) {
                if (dvMessagesExist(oldName, connection->sourceName(), connection->targetName(), msgEnd)) {
                    messagesData << QStringList({ connection->sourceName(), connection->targetName() });
                }
            }
        }

        if (!messagesData.isEmpty()) {
            if (command->isFirstChange()) {
                const int result = QMessageBox::question(
                        nullptr, tr("Update messages"), tr("Do you want to update MSC messages / DV bindings?"));
                if (result == QMessageBox::Yes) {
                    for (const QStringList &data : qAsConst(messagesData)) {
                        changeMscMessageName(oldName, entity->title(), data[0], data[1]);
                        changeDvMessageBindingName(oldName, entity->title(), data[0], data[1], msgEnd);
                    }
                }
            } else {
                for (const QStringList &data : qAsConst(messagesData)) {
                    changeMscMessageName(oldName, entity->title(), data[0], data[1]);
                    changeDvMessageBindingName(oldName, entity->title(), data[0], data[1], msgEnd);
                }
            }
        }
    }
}

/*!
   Checks if iv function and connection needs to be updated/added
 */
void MscSystemChecks::onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command)
{
    if (m_nameUpdateRunning) {
        return;
    }
    IVEditorCorePtr ivCore = m_storage->ivCore();
    if (!ivCore) {
        return;
    }

    auto instance = dynamic_cast<msc::MscInstance *>(entity);
    if (instance) {
        const bool hasOldName = ivCore->ivFunctionsNames().contains(oldName, m_caseCheck);
        const bool hasNewName = ivCore->ivFunctionsNames().contains(instance->name(), m_caseCheck);

        if (!hasOldName && !hasNewName) {
            const int result = QMessageBox::question(nullptr, tr("No IV function"),
                    tr("The IV model doesn't contain a function called:\n%1\n"
                       "\nDo you want to add it to the IV model?")
                            .arg(instance->name()));
            if (result == QMessageBox::Yes) {
                ivCore->addFunction(instance->name());
            }
        }

        if (hasOldName && !hasNewName) {
            if (command && command->isFirstChange()) {
                QMessageBox box;
                box.setWindowTitle(tr("Update IV function"));
                box.setText(tr("The IV/DV function should be updated."
                               "\nDo you want to update it?"
                               "\nDo you want to add it to the IV model?"));
                QPushButton *updateButton = box.addButton(tr("Update"), QMessageBox::AcceptRole);
                QPushButton *addButton = box.addButton(tr("Add"), QMessageBox::AcceptRole);
                box.addButton(tr("Ignore"), QMessageBox::RejectRole);
                box.setDefaultButton(updateButton);
                box.exec();
                if (box.clickedButton() == updateButton) {
                    m_nameUpdateRunning = true;
                    ivCore->renameIVFunction(oldName, instance->name());
                    changeMscInstanceName(oldName, instance->name());
                    changeDvFunctionBindingName(oldName, instance->name());
                    m_nameUpdateRunning = false;
                }
                if (box.clickedButton() == addButton) {
                    ivCore->addFunction(instance->name());
                }
            } else {
                m_nameUpdateRunning = true;
                ivCore->renameIVFunction(oldName, instance->name());
                changeMscInstanceName(oldName, instance->name());
                changeDvFunctionBindingName(oldName, instance->name());

                m_nameUpdateRunning = false;
            }
        }
    }

    auto message = dynamic_cast<msc::MscMessage *>(entity);
    if (message && ivCore && ivCore->document() && ivCore->document()->objectsModel()) {
        scs::IvSystemChecks ivChecker(nullptr);
        ivChecker.setIvCore(ivCore);
        // Check if source/target are in IV model
        if (message->sourceInstance() && !ivChecker.checkInstance(message->sourceInstance())) {
            return;
        }
        if (message->targetInstance() && !ivChecker.checkInstance(message->targetInstance())) {
            return;
        }

        // Check for names
        const QString fromName = message->sourceInstance() ? message->sourceInstance()->name() : "";
        const QString toName = message->targetInstance() ? message->targetInstance()->name() : "";

        bool hasNewName = ivChecker.checkMessage(message);
        msc::MscMessage oldMessage(oldName);
        oldMessage.setSourceInstance(message->sourceInstance());
        oldMessage.setTargetInstance(message->targetInstance());
        bool hasOldName = ivChecker.checkMessage(&oldMessage);

        if (!hasNewName && !hasOldName) {
            const int result = QMessageBox::question(nullptr, tr("No IV connection"),
                    tr("The IV model doesn't contain a connection called:\n%1\n"
                       "\nDo you want to add it to the IV model?")
                            .arg(message->name()));
            if (result == QMessageBox::Yes) {
                ivCore->addConnection(message->name(), fromName, toName);
            }
        }

        if (hasOldName && !hasNewName) {
            if (command->isFirstChange()) {
                QMessageBox box;
                box.setWindowTitle(tr("Update IV connection"));
                box.setText(tr("The IV connection should be updated."
                               "\nDo you want to update it?"
                               "\nDo you want to add it to the IV model?"));
                QPushButton *updateButton = box.addButton(tr("Update"), QMessageBox::AcceptRole);
                QPushButton *addButton = box.addButton(tr("Add"), QMessageBox::AcceptRole);
                box.addButton(tr("Ignore"), QMessageBox::RejectRole);
                box.setDefaultButton(updateButton);
                box.exec();
                if (box.clickedButton() == updateButton) {
                    m_nameUpdateRunning = true;
                    ivCore->renameIVConnection(oldName, message->name(), fromName, toName);
                    changeMscMessageName(oldName, message->name(), fromName, toName);
                    changeDvMessageBindingName(oldName, message->name(), fromName, toName, shared::TARGET);
                    m_nameUpdateRunning = false;
                }
                if (box.clickedButton() == addButton) {
                    ivCore->addConnection(message->name(), fromName, toName);
                }
            } else {
                m_nameUpdateRunning = true;
                ivCore->renameIVConnection(oldName, message->name(), fromName, toName);
                changeMscMessageName(oldName, message->name(), fromName, toName);
                changeDvMessageBindingName(oldName, message->name(), fromName, toName, shared::TARGET);
                m_nameUpdateRunning = false;
            }
        }
    }
}

/*!
   Removes corresponding MSC entities when a IV entity was removed
 */
void MscSystemChecks::onEntitiesRemoved(const QList<QPointer<ivm::IVObject>> &entities, shared::UndoCommand *command)
{
    if (entities.isEmpty()) {
        return;
    }

    QStringList removedNames;
    QList<ivm::IVFunction *> removedFunctions;
    QList<ivm::IVConnection *> removedConnections;
    for (const QPointer<ivm::IVObject> &entity : entities) {
        if (!entity) {
            continue;
        }
        if (entity->type() == ivm::IVObject::Type::Function) {
            removedFunctions.append(qobject_cast<ivm::IVFunction *>(entity));
        } else if (entity->type() == ivm::IVObject::Type::Connection) {
            removedConnections.append(qobject_cast<ivm::IVConnection *>(entity));
        } else {
            continue;
        }
        removedNames.append(entity->title());
    }

    if (removedNames.isEmpty()) {
        return;
    }

    bool doRemove = true;
    if (command->isFirstChange()) {
        const int result = QMessageBox::question(nullptr, tr("Remove MSC/DV entities"),
                tr("The IV entity(ies) %1 was(were) removed."
                   "\nDo you want to remove the correlating MSC/DV entities?")
                        .arg(removedNames.join(QLatin1String(", "))));
        if (result != QMessageBox::Yes) {
            doRemove = false;
        }
    }
    if (doRemove) {
        for (auto ivFunction : qAsConst(removedFunctions)) {
            removeMscInstances(ivFunction);
            removeDvFunctionBinding(ivFunction);
        }
        for (auto ivConnection : qAsConst(removedConnections)) {
            removeMscMessages(ivConnection);
            removeDvMessageBinding(ivConnection);
        }
    }
}

void MscSystemChecks::onImplementationChanged(
        ivm::IVFunction *entity, const QString &newName, const QString &oldName, shared::UndoCommand *command)
{
    Q_UNUSED(command);
    /// @todo ask user to update
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeFunctionImplementationName(entity->title(), newName, oldName);
    }
}

void MscSystemChecks::onDefaultImplementationChanged()
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDefaultImplementationNames();
    }
}

}
