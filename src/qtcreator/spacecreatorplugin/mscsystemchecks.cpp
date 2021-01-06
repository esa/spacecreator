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

#include "aadlconnectionchain.h"
#include "aadlmodelstorage.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "aadlsystemchecks.h"
#include "chartlayoutmanager.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdifaceattrchange.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscmodelstorage.h"
#include "undocommand.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QUndoStack>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <utils/fileutils.h>

namespace spctr {

MscSystemChecks::MscSystemChecks(QObject *parent)
    : QObject(parent)
{
}

void MscSystemChecks::setMscStorage(MscModelStorage *mscStorage)
{
    m_mscStorage = mscStorage;
    connect(m_mscStorage, &spctr::MscModelStorage::coreAdded, this, [=](QSharedPointer<msc::MSCEditorCore> core) {
        connect(core.data(), &msc::MSCEditorCore::nameChanged, this, &spctr::MscSystemChecks::onMscEntityNameChanged);
    });
}

void MscSystemChecks::setAadlStorage(AadlModelStorage *aadlStorage)
{
    m_aadlStorage = aadlStorage;
}

/*!
   Returns if at least one instance in one of the .msc files has the name \p name
 */
bool MscSystemChecks::mscInstancesExist(const QString &name)
{
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscInstance *instance : chart->instances()) {
                if (instance->name() == name) {
                    return true;
                }
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
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
        mscCore->changeMscInstanceName(oldName, name);
    }
}

/*!
   Removes all instance that are corresponding to the function \p aaldFunction
 */
void MscSystemChecks::removeMscInstances(aadl::AADLObjectFunction *aadlFunction)
{
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
        mscCore->removeMscInstances(aadlFunction);
    }
}

/*!
  Returns if at least one message in one of the .msc files has the name \p messageName
 */
bool MscSystemChecks::mscMessagesExist(const QString &messageName, const QString &sourceName, const QString &targetName)
{
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
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
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
        mscCore->changeMscMessageName(oldName, newName, sourceName, targetName);
    }
}

/*!
   Removes all messages that are corresponding to the connection \p aadlConnection
 */
void MscSystemChecks::removeMscMessages(aadl::AADLObjectConnection *aadlConnection)
{
    for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
        mscCore->removeMscMessages(aadlConnection);
    }
}

/*!
   Checks if all instances have corresponding functions in the aadl model
   Errors are reported in a message box
 */
void MscSystemChecks::checkInstances()
{
    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    // Check for names
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultNames;
    for (QSharedPointer<msc::MSCEditorCore> &mplugin : mscCores) {
        resultNames += mplugin->aadlChecker()->checkInstanceNames();
    }

    // Check for nested functions usage
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultRelations;
    for (QSharedPointer<msc::MSCEditorCore> &mplugin : mscCores) {
        resultRelations += mplugin->aadlChecker()->checkInstanceRelations();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following instances have no corresponding aadl function:\n");
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
   Checks if all messages have corresponding connections in the aadl model
   Errors are reported in a message box
 */
void MscSystemChecks::checkMessages()
{
    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    // check messages
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> resultNames;
    for (const QSharedPointer<msc::MSCEditorCore> &mplugin : mscCores) {
        resultNames += mplugin->aadlChecker()->checkMessages();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following messages have no corresponding aadl connection:\n");
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

QSharedPointer<aadlinterface::IVEditorCore> MscSystemChecks::ivCore() const
{
    QStringList aadlFiles = allAadlFiles();
    if (aadlFiles.empty()) {
        qWarning() << "No AADL file in the project";
        return {};
    }

    return m_aadlStorage->ivData(aadlFiles.first());
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<QSharedPointer<msc::MSCEditorCore>> MscSystemChecks::allMscCores() const
{
    QStringList mscFiles = allMscFiles();
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores;
    for (const QString &mscFile : mscFiles) {
        QSharedPointer<msc::MSCEditorCore> core = m_mscStorage->mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

/*!
   Returns all aald files of the current project
 */
QStringList MscSystemChecks::allAadlFiles()
{
    return projectFiles("interfaceview.xml");
}

/*!
   Returns all msc files of the current project
 */
QStringList MscSystemChecks::allMscFiles()
{
    return projectFiles(".msc");
}

/*!
   Returns all asn files of the current project
 */
QStringList MscSystemChecks::allAsn1Files()
{
    return projectFiles(".asn");
}

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList MscSystemChecks::projectFiles(const QString &suffix)
{
    ProjectExplorer::Project *project = ProjectExplorer::ProjectTree::currentProject();
    if (!project) {
        return {};
    }

    QStringList result;
    for (const Utils::FileName &fileName : project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }

    return result;
}

void MscSystemChecks::onEntityNameChanged(
        aadl::AADLObject *entity, const QString &oldName, shared::UndoCommand *command)
{
    if (m_nameUpdateRunning) {
        return;
    }

    auto cmdAttribChange = dynamic_cast<aadlinterface::cmd::CmdEntityAttributeChange *>(command);
    if (cmdAttribChange) {
        if (mscInstancesExist(oldName)) {
            if (command->isFirstChange()) {
                const int result = QMessageBox::question(
                        nullptr, tr("Update instances"), tr("Do you want to update MSC instances?"));
                if (result == QMessageBox::Yes) {
                    m_nameUpdateRunning = true;
                    changeMscInstanceName(oldName, entity->title());
                    m_nameUpdateRunning = false;
                }
            } else {
                changeMscInstanceName(oldName, entity->title());
            }
        }
    }

    auto cmdIfaceAttribChange = dynamic_cast<aadlinterface::cmd::CmdIfaceAttrChange *>(command);
    if (cmdIfaceAttribChange
            && cmdIfaceAttribChange->interface()->direction() == aadl::AADLObjectIface::IfaceType::Provided) {
        QVector<aadl::AADLObjectConnection *> connections = cmdIfaceAttribChange->getRelatedConnections();
        for (const aadl::AADLObjectConnection *connection : connections) {
            if (mscMessagesExist(oldName, connection->sourceName(), connection->targetName())) {
                if (command->isFirstChange()) {
                    const int result = QMessageBox::question(
                            nullptr, tr("Update messages"), tr("Do you want to update MSC messages?"));
                    if (result == QMessageBox::Yes) {
                        changeMscMessageName(
                                oldName, entity->title(), connection->sourceName(), connection->targetName());
                    }
                } else {
                    changeMscMessageName(oldName, entity->title(), connection->sourceName(), connection->targetName());
                }
            }
        }
    }
}

/*!
   Checks if aadl function and connection needs to be updated/added
 */
void MscSystemChecks::onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command)
{
    if (m_nameUpdateRunning) {
        return;
    }
    if (!ivCore()) {
        return;
    }

    auto instance = dynamic_cast<msc::MscInstance *>(entity);
    if (instance) {
        const bool hasOldName = ivCore()->aadlFunctionsNames().contains(oldName, m_caseCheck);
        const bool hasNewName = ivCore()->aadlFunctionsNames().contains(instance->name(), m_caseCheck);

        if (!hasOldName && !hasNewName) {
            const int result = QMessageBox::question(nullptr, tr("No AADL function"),
                    tr("The AADL model doesn't contain a function called:\n%1\n"
                       "\nDo you want to add it to the AADL model?")
                            .arg(instance->name()));
            if (result == QMessageBox::Yes) {
                ivCore()->addFunction(instance->name());
            }
        }

        if (hasOldName && !hasNewName) {
            if (command->isFirstChange()) {
                QMessageBox box;
                box.setWindowTitle(tr("Update AADL function"));
                box.setText(tr("The AADL function should be updated."
                               "\nDo you want to update it?"
                               "\nDo you want to add it to the AADL model?"));
                QPushButton *updateButton = box.addButton(tr("Update"), QMessageBox::AcceptRole);
                QPushButton *addButton = box.addButton(tr("Add"), QMessageBox::AcceptRole);
                box.addButton(tr("Ignore"), QMessageBox::RejectRole);
                box.setDefaultButton(updateButton);
                box.exec();
                if (box.clickedButton() == updateButton) {
                    m_nameUpdateRunning = true;
                    ivCore()->renameAadlFunction(oldName, instance->name());
                    changeMscInstanceName(oldName, instance->name());
                    m_nameUpdateRunning = false;
                }
                if (box.clickedButton() == addButton) {
                    ivCore()->addFunction(instance->name());
                }
            } else {
                m_nameUpdateRunning = true;
                ivCore()->renameAadlFunction(oldName, instance->name());
                changeMscInstanceName(oldName, instance->name());
                m_nameUpdateRunning = false;
            }
        }
    }

    auto message = dynamic_cast<msc::MscMessage *>(entity);
    if (message && ivCore() && ivCore()->document() && ivCore()->document()->objectsModel()) {
        // Check for names
        const QString fromName = message->sourceInstance() ? message->sourceInstance()->name() : "";
        const QString toName = message->targetInstance() ? message->targetInstance()->name() : "";
        msc::AadlSystemChecks aadlChecker;
        aadlChecker.setIvCore(ivCore());
        bool hasNewName = aadlChecker.checkMessage(message);
        msc::MscMessage oldMessage(oldName);
        oldMessage.setSourceInstance(message->sourceInstance());
        oldMessage.setTargetInstance(message->targetInstance());
        bool hasOldName = aadlChecker.checkMessage(&oldMessage);

        if (!hasNewName && !hasOldName) {
            const int result = QMessageBox::question(nullptr, tr("No AADL connection"),
                    tr("The AADL model doesn't contain a connection called:\n%1\n"
                       "\nDo you want to add it to the AADL model?")
                            .arg(message->name()));
            if (result == QMessageBox::Yes) {
                ivCore()->addConnection(message->name(), fromName, toName);
            }
        }

        if (hasOldName && !hasNewName) {
            if (command->isFirstChange()) {
                QMessageBox box;
                box.setWindowTitle(tr("Update AADL connection"));
                box.setText(tr("The AADL connection should be updated."
                               "\nDo you want to update it?"
                               "\nDo you want to add it to the AADL model?"));
                QPushButton *updateButton = box.addButton(tr("Update"), QMessageBox::AcceptRole);
                QPushButton *addButton = box.addButton(tr("Add"), QMessageBox::AcceptRole);
                box.addButton(tr("Ignore"), QMessageBox::RejectRole);
                box.setDefaultButton(updateButton);
                box.exec();
                if (box.clickedButton() == updateButton) {
                    m_nameUpdateRunning = true;
                    ivCore()->renameAadlConnection(oldName, message->name(), fromName, toName);
                    changeMscMessageName(oldName, message->name(), fromName, toName);
                    m_nameUpdateRunning = false;
                }
                if (box.clickedButton() == addButton) {
                    ivCore()->addConnection(message->name(), fromName, toName);
                }
            } else {
                m_nameUpdateRunning = true;
                ivCore()->renameAadlConnection(oldName, message->name(), fromName, toName);
                changeMscMessageName(oldName, message->name(), fromName, toName);
                m_nameUpdateRunning = false;
            }
        }
    }
}

/*!
   Removes corresponding MSC entities when a AADL entity wasremoved
 */
void MscSystemChecks::onEntityRemoved(aadl::AADLObject *entity, shared::UndoCommand *command)
{
    if (!entity) {
        return;
    }

    bool doRemove = true;
    if (command->isFirstChange()) {
        const int result = QMessageBox::question(nullptr, tr("Remove MSC entities"),
                tr("The AADL entity %1 was removed."
                   "\nDo you want to remove the correlating MSC entities?")
                        .arg(entity->title()));
        if (result != QMessageBox::Yes) {
            doRemove = false;
        }
    }
    if (doRemove) {
        auto aadlFunction = dynamic_cast<aadl::AADLObjectFunction *>(entity);
        if (aadlFunction) {
            removeMscInstances(aadlFunction);
        }
        auto aadlConnection = dynamic_cast<aadl::AADLObjectConnection *>(entity);
        if (aadlConnection) {
            removeMscMessages(aadlConnection);
        }
    }
}

}
