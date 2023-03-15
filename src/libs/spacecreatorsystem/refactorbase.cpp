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

#include "refactorbase.h"

#include "ivinterface.h"

#include <ivconnection.h>

namespace scs {

/*!
 * Returns if the type of refactoring is supported by this class
 */
bool RefactorBase::isRefactorSupported(RefactorType type) const
{
    Q_UNUSED(type);
    return false;
}

/*!
 * Returns if the given IV function is "used" (in any kind) in any of the files (type) this class handles
 * \param func The function to check if it it used
 * \param name The name of the function (the name of function might be wrong)
 */
bool RefactorBase::isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const
{
    Q_UNUSED(name);
    Q_UNUSED(func);
    return false;
}

/*!
 * Returns if the given IV interface is "used" (in any kind) in any of the files (type) this class handles
 * \param func The interface to check if it it used
 * \param name The name of the interface (the name of interface might be wrong)
 */
bool RefactorBase::isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const
{
    Q_UNUSED(interface);
    Q_UNUSED(name);
    return false;
}

/*!
 * Returns if the given IV connection is "used" (in any kind) in any of the files (type) this class handles
 * \param connection The connection to check if it is used
 */
bool RefactorBase::isIVConnectionUsed(ivm::IVConnection *connection) const
{
    Q_UNUSED(connection);
    return false;
}

/*!
 * Sets the name of all objects corresponding to the IV function used in any of the files (type) this class handles to
 * \p newName
 *
 * \param func The function that was renamed
 * \param oldName The old name
 * \param newName The new (now current)
 * name
 */
void RefactorBase::onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const
{
    Q_UNUSED(func);
    Q_UNUSED(oldName);
    Q_UNUSED(newName);
}

/*!
 * Sets the name of all objects corresponding to the IV interfaceused in any of the files (type) this class handles to
 * \p newName
 *
 * \param interface The interface that was renamed
 * \param oldName The old name
 * \param newName The new (now current) name
 */
void RefactorBase::onIVInterfaceRenamed(
        ivm::IVInterface *interface, const QString &oldName, const QString &newName) const
{
    Q_UNUSED(interface);
    Q_UNUSED(oldName);
    Q_UNUSED(newName);
}

/*!
 * This is called while the IV object \p obj is removed. So corresponding objects are removed as well.
 */
void RefactorBase::onRemovingIVObject(ivm::IVObject *obj) const
{
    Q_UNUSED(obj);
}

/*!
 * Updates all implementation names to the new one \see ive::IVFunction::implementations \see
 * ive::IVFunction::addImplementation
 *
 * \param entity The object where one iomplemenation name was changed
 * \param newName The new name of the implementation
 * \param oldName The old name of the implementation
 */
void RefactorBase::onImplementationChanged(ivm::IVFunction *entity, const QString &newName, const QString &oldName)
{
    Q_UNUSED(entity);
    Q_UNUSED(newName);
    Q_UNUSED(oldName);
}

/*!
 * Updates the default implemenations of all interfaces \see ive::IVFunction::defaultImplementation
 */
void RefactorBase::onDefaultImplementationChanged()
{
    return;
}

/*!
 * Updates the whole list of implementations of one IVFunction \see ive::IVFunction::implementations
 *
 * \param ivFunction The function to update the implementations from
 */
void RefactorBase::onImplementationListChanged(ivm::IVFunction *ivFunction)
{
    Q_UNUSED(ivFunction);
}

} // namespace scs
