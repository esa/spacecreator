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

#include "mscrefactor.h"

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "spacecreatorproject.h"

namespace scs {

/*!
 * \see RefactorBase::isRefactorSupported
 */
bool MSCRefactor::isRefactorSupported(RefactorType type) const
{
    switch (type) {
    case RefactorBase::IVFunctionRename:
    case RefactorBase::IVInterfaceRename:
    case RefactorBase::IVObjectRemoved:
        return true;
    default:
        break;
    }

    return false;
}

/*!
 * \see RefactorBase::isIVFunctionUsed
 */
bool MSCRefactor::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    Q_UNUSED(func);
    Q_ASSERT(!m_storage.isNull());
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
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
 * \see RefactorBase::isIVInterfaceUsed
 */
bool MSCRefactor::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    if (interface->direction() != ivm::IVInterface::InterfaceType::Provided) {
        return false;
    }

    return !correspondingMessages(interface, name).isEmpty();
}

/*!
 * \see RefactorBase::isIVConnectionUsed
 */
bool MSCRefactor::isIVConnectionUsed(ivm::IVConnection *connection) const
{
    ivm::IVInterface *iface = connection->targetInterface();
    return isIVInterfaceUsed(iface, iface->title());
}

/*!
 * \see RefactorBase::onIvFunctionRenamed
 */
void MSCRefactor::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    Q_UNUSED(func);
    Q_ASSERT(!m_storage.isNull());
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        mscCore->changeMscInstanceName(oldName, newName);
    }
}

/*!
 * \see RefactorBase::onIvInterfaceRenamed
 */
void MSCRefactor::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    QSet<QPair<QString, QString>> sourceTargets;
    QList<msc::MscMessage *> messages = correspondingMessages(interface, oldName);
    for (msc::MscMessage *message : messages) {
        sourceTargets.insert({ message->sourceName(), message->targetName() });
    }

    for (const QPair<QString, QString> &pair : sourceTargets) {
        for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
            mscCore->changeMscMessageName(oldName, newName, pair.first, pair.second);
        }
    }
}

/*!
 * \see RefactorBase::onIVObjectRemoved
 */
void MSCRefactor::onRemovingIVObject(ivm::IVObject *obj) const
{
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        switch (obj->type()) {
        case ivm::IVObject::Type::Function:
            mscCore->removeMscInstances(static_cast<ivm::IVFunction *>(obj));
            break;
        case ivm::IVObject::Type::Connection:
            mscCore->removeMscMessages(static_cast<ivm::IVConnection *>(obj));
            break;
        default:
            break;
        }
    }
}

/*!
 * \todo move to a class MscSystemQueries
 */
QList<msc::MscMessage *> MSCRefactor::correspondingMessages(ivm::IVInterface *interface, const QString &name) const
{
    Q_ASSERT(interface != nullptr);
    Q_ASSERT(interface->model() != nullptr);
    QList<msc::MscMessage *> messages;

    if (interface->direction() != ivm::IVInterface::InterfaceType::Provided) {
        return messages;
    }

    QVector<ivm::IVConnection *> connections = interface->model()->getRelatedConnections(interface);
    for (const ivm::IVConnection *connection : qAsConst(connections)) {
        messages.append(allMessages(name, connection->sourceName(), connection->targetName()));
    }
    // Update from cyclic interfaces
    ivm::IVFunctionType *func = interface->function();
    if (func && interface->kind() == ivm::IVInterface::OperationKind::Cyclic) {
        messages.append(allMessages(name, func->title(), ""));
        messages.append(allMessages(name, "", func->title()));
    }

    return messages;
}

/*!
 * Returns all messges that have the names of the message, source and target as in the parameters
 */
QList<msc::MscMessage *> MSCRefactor::allMessages(
        const QString &messageName, const QString &sourceName, const QString &targetName) const
{
    QList<msc::MscMessage *> messages;
    for (MSCEditorCorePtr &mscCore : m_storage->allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscMessage *message : chart->messages()) {
                if (message->name() == messageName && message->sourceName() == sourceName
                        && message->targetName() == targetName) {
                    messages.append(message);
                }
            }
        }
    }
    return messages;
}

} // namespace scs
