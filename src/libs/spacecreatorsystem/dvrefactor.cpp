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

#include "dvrefactor.h"

#include "dvappmodel.h"
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "spacecreatorproject.h"

#include <dvsystemqueries.h>

namespace scs {

void DVRefactor::setStorage(SpaceCreatorProject *storage)
{
    m_storage = storage;
}

/*!
 * \see RefactorBase::isRefactorSupported
 */
bool DVRefactor::isRefactorSupported(RefactorType type) const
{
    return true;
}

/*!
 * \see RefactorBase::isIVFunctionUsed
 */
bool DVRefactor::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    Q_UNUSED(func);
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
 * \see RefactorBase::isIVInterfaceUsed
 */
bool DVRefactor::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    return !correspondingMessages(interface, name).isEmpty();
}

/*!
 * \see RefactorBase::isIVConnectionUsed
 */
bool DVRefactor::isIVConnectionUsed(ivm::IVConnection *connection) const
{
    ivm::IVInterface *iface = connection->targetInterface();
    return isIVInterfaceUsed(iface, iface->title());
}

/*!
 * \see RefactorBase::onIvFunctionRenamed
 */
void DVRefactor::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    Q_UNUSED(func);
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDvFunctionBindingName(oldName, newName);
    }
}

/*!
 * \see RefactorBase::onIvInterfaceRenamed
 */
void DVRefactor::onIVInterfaceRenamed(ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    QSet<QPair<QString, QString>> sourceTargets;
    QList<dvm::DVMessage *> messages = correspondingMessages(interface, oldName);
    for (dvm::DVMessage *message : messages) {
        sourceTargets.insert({ message->fromFunction(), message->toFunction() });
    }

    shared::MessageEnd msgSide =
            interface->direction() == ivm::IVInterface::InterfaceType::Provided ? shared::TARGET : shared::SOURCE;
    for (const QPair<QString, QString> &pair : sourceTargets) {
        for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
            dvCore->changeDvMessageBindingIfName(oldName, newName, pair.first, pair.second, msgSide);
        }
    }
}

/*!
 * \see RefactorBase::onIVObjectRemoved
 */
void DVRefactor::onRemovingIVObject(ivm::IVObject *obj) const
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        switch (obj->type()) {
        case ivm::IVObject::Type::Function:
            dvCore->removeDvFunctionBinding(obj->title());
            break;
        case ivm::IVObject::Type::Connection: {
            ivm::IVConnection *ivConnection = static_cast<ivm::IVConnection *>(obj);
            dvCore->removeDvMessageBinding(ivConnection->sourceName(), ivConnection->sourceInterfaceName(),
                    ivConnection->targetName(), ivConnection->targetInterfaceName());
            break;
        }
        default:
            break;
        }
    }
}

/*!
 * \see RefactorBase::onImplementationChanged
 */
void DVRefactor::onImplementationChanged(ivm::IVFunction *entity, const QString &newName, const QString &oldName)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeFunctionImplementationName(entity->title(), newName, oldName);
    }
}

/*!
 * \see RefactorBase::onDefaultImplementationChanged
 */
void DVRefactor::onDefaultImplementationChanged()
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->changeDefaultImplementationNames();
    }
}

/*!
 * \see RefactorBase::onImplementationListChanged
 */
void DVRefactor::onImplementationListChanged(ivm::IVFunction *ivFunction)
{
    for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
        dvCore->updateFunctionImplementationList(ivFunction->title());
    }
}

/*!
 * \see RefactorBase::onAttributeChanged
 */
void DVRefactor::onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue)
{
    Q_UNUSED(oldValue)
    Q_UNUSED(entity)

    static QString kindToken = ivm::meta::Props::token(ivm::meta::Props::Token::kind);
    if (attrName == kindToken) {
        DvSystemQueries dvChecks;
        dvChecks.setStorage(m_storage);
        for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
            dvChecks.checkDVFile(dvCore);
        }
    }
}

/*!
 * \todo move o a class DVSystemQueries
 */
QList<dvm::DVMessage *> DVRefactor::correspondingMessages(ivm::IVInterface *interface, const QString &name) const
{
    QList<dvm::DVMessage *> messages;

    QVector<ivm::IVConnection *> connections = interface->model()->getRelatedConnections(interface);
    shared::MessageEnd msgSide =
            interface->direction() == ivm::IVInterface::InterfaceType::Provided ? shared::TARGET : shared::SOURCE;

    for (const ivm::IVConnection *connection : qAsConst(connections)) {
        for (const DVEditorCorePtr &dvCore : m_storage->allDVCores()) {
            dvm::DVModel *model = dvCore->appModel()->objectsModel();
            for (dvm::DVMessage *msg : model->allObjectsByType<dvm::DVMessage>()) {
                if (msg->fromFunction() == connection->sourceName() && msg->toFunction() == connection->targetName()) {
                    if ((msgSide == shared::SOURCE || msgSide == shared::BOTH) && msg->fromInterface() == name) {
                        messages.append(msg);
                    }
                    if ((msgSide == shared::TARGET || msgSide == shared::BOTH) && msg->toInterface() == name) {
                        messages.append(msg);
                    }
                }
            }
        }
    }

    return messages;
}

} // namespace scs
