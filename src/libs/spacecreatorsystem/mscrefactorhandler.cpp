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

#include "mscrefactorhandler.h"

#include "dveditorcore.h"
#include "interfacedocument.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "ivsystemchecks.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "spacecreatorproject.h"
#include "undocommand.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QUndoStack>

namespace scs {

MscRefactorHandler::MscRefactorHandler(QObject *parent)
    : QObject(parent)
{
}

void MscRefactorHandler::setStorage(SpaceCreatorProject *storage)
{
    m_storage = storage;

    connect(m_storage, &scs::SpaceCreatorProject::mscCoreAdded, this, [=](MSCEditorCorePtr core) {
        connect(core.data(), &msc::MSCEditorCore::nameChanged, this, &scs::MscRefactorHandler::onMscEntityNameChanged);
    });
}

/*!
   Changes all instances that have the name \p oldName to have the new name \p name
 */
void MscRefactorHandler::changeMscInstanceName(const QString &oldName, const QString &name)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->changeMscInstanceName(oldName, name);
    }
}

/*!
   Changes all messages that have the name \p oldName to have the new name \p newName, if the source and target have the
   names \p sourceName and \p targetName
 */
void MscRefactorHandler::changeMscMessageName(
        const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName)
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->changeMscMessageName(oldName, newName, sourceName, targetName);
    }
}

/*!
   Changes all function bindings that have the name \p oldName to have the new name \p name
 */
void MscRefactorHandler::changeDvFunctionBindingName(const QString &oldName, const QString &name)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDvFunctionBindingName(oldName, name);
    }
}

/*!
   Update all DV message bindings interface name, that match the name, source/target function and message side/end
 */
void MscRefactorHandler::changeDvMessageBindingName(const QString &oldName, const QString &name,
        const QString &sourceName, const QString &targetName, shared::MessageEnd msgSide)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDvMessageBindingIfName(oldName, name, sourceName, targetName, msgSide);
    }
}

/*!
   Checks if iv function and connection needs to be updated/added
 */
void MscRefactorHandler::onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command)
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

}
