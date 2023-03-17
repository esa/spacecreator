/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivrefactorhandler.h"

#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdifaceattrchange.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "refactorbase.h"

#include <QMessageBox>
#include <algorithm>

namespace scs {

IVRefactorHandler::IVRefactorHandler(QObject *parent)
    : QObject { parent }
{
}

void IVRefactorHandler::registerRefactor(RefactorBase *refactor)
{
    if (!m_refactors.contains(refactor)) {
        m_refactors.append(refactor);
    }
}

void IVRefactorHandler::onIVEntityNameChanged(
        ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command)
{
    Q_ASSERT(entity);
    Q_ASSERT(command);

    if (entity->title() == oldName) {
        return;
    }

    auto cmdAttribChange = dynamic_cast<ive::cmd::CmdFunctionAttrChange *>(command);
    ivm::IVFunction *func = dynamic_cast<ivm::IVFunction *>(entity);
    if (cmdAttribChange && func) {
        onIVFunctionNameChanged(func, oldName, cmdAttribChange);
        return;
    }

    auto cmdIfaceAttribChange = dynamic_cast<ive::cmd::CmdIfaceAttrChange *>(command);
    ivm::IVInterface *interface = dynamic_cast<ivm::IVInterface *>(entity);
    if (cmdIfaceAttribChange && interface) {
        onIVInterfaceNameChanged(interface, oldName, cmdIfaceAttribChange);
        return;
    }
}

void IVRefactorHandler::onEntitiesRemoved(const QList<QPointer<ivm::IVObject>> &entities, shared::UndoCommand *command)
{
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
    }

    if (removedFunctions.isEmpty() && removedConnections.isEmpty()) {
        return;
    }

    bool used = false;
    for (ivm::IVFunction *func : removedFunctions) {
        used |= std::any_of(m_refactors.begin(), m_refactors.end(),
                [func](RefactorBase *refactor) { return refactor->isIVFunctionUsed(func, func->title()); });
    }
    for (ivm::IVConnection *connection : removedConnections) {
        used |= std::any_of(m_refactors.begin(), m_refactors.end(),
                [connection](RefactorBase *refactor) { return refactor->isIVConnectionUsed(connection); });
    }
    if (!used) {
        return;
    }

    if (command->isFirstChange()) {
        const int result = QMessageBox::question(nullptr, tr("Remove entities"),
                tr("Do you want to remove the entity(ies) in other documents?", "",
                        removedFunctions.size() + removedConnections.size()));
        if (result != QMessageBox::Yes) {
            return;
        }
    }

    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVObjectRemoved)) {
            for (ivm::IVFunction *func : removedFunctions) {
                refactor->onRemovingIVObject(func);
            }
            for (ivm::IVConnection *connection : removedConnections) {
                refactor->onRemovingIVObject(connection);
            }
        }
    }
}

void IVRefactorHandler::onImplementationChanged(
        ivm::IVFunction *entity, const QString &newName, const QString &oldName, shared::UndoCommand *command)
{
    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVFunctionImplementationChanged)) {
            refactor->onImplementationChanged(entity, newName, oldName);
        }
    }
}

void IVRefactorHandler::onDefaultImplementationChanged()
{
    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVFunctionImplementationChanged)) {
            refactor->onDefaultImplementationChanged();
        }
    }
}

void IVRefactorHandler::onImplementationListChanged(ivm::IVFunction *ivFunction)
{
    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVFunctionImplementationChanged)) {
            refactor->onImplementationListChanged(ivFunction);
        }
    }
}

void IVRefactorHandler::onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue)
{
    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVAttributeChanged)) {
            refactor->onAttributeChanged(entity, attrName, oldValue);
        }
    }
}

void IVRefactorHandler::onIVFunctionNameChanged(
        ivm::IVFunction *func, const QString &oldName, ive::cmd::CmdFunctionAttrChange *command)
{
    if (!std::any_of(m_refactors.begin(), m_refactors.end(),
                [func, &oldName](RefactorBase *refactor) { return refactor->isIVFunctionUsed(func, oldName); })) {
        return;
    }

    if (command->isFirstChange()) {
        const int result = QMessageBox::question(
                nullptr, tr("Refactor?"), tr("Do you want to update the function name in other documents?"));
        if (result != QMessageBox::Yes) {
            return;
        }
    }

    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVFunctionRename)) {
            refactor->onIVFunctionRenamed(func, oldName, func->title());
        }
    }
}

void IVRefactorHandler::onIVInterfaceNameChanged(
        ivm::IVInterface *interface, const QString &oldName, ive::cmd::CmdIfaceAttrChange *command)
{
    if (!std::any_of(m_refactors.begin(), m_refactors.end(), [interface, &oldName](RefactorBase *refactor) {
            return refactor->isIVInterfaceUsed(interface, oldName);
        })) {
        return;
    }

    if (command->isFirstChange()) {
        const int result = QMessageBox::question(nullptr, tr("Update connections"),
                tr("Do you want to update the connections/messages name in other documents?"));
        if (result != QMessageBox::Yes) {
            return;
        }
    }

    for (RefactorBase *refactor : m_refactors) {
        if (refactor->isRefactorSupported(scs::RefactorBase::IVInterfaceRename)) {
            refactor->onIVInterfaceRenamed(interface, oldName, interface->title());
        }
    }
}

} // namespace scs
