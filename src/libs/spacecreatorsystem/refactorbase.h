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

#pragma once

#include <QPointer>
#include <QString>

namespace ivm {
class IVConnection;
class IVFunction;
class IVInterface;
class IVObject;
}

namespace shared {
class VEObject;
}

namespace scs {
class SpaceCreatorProject;

/**
 * Base class for all classes that implement the refactoring for one specific file type
 */
class RefactorBase
{
public:
    enum RefactorType
    {
        IVFunctionRename,
        IVInterfaceRename,
        IVObjectRemoved,
        IVFunctionImplementationChanged,
        IVDefaultImplementationChanged,
        IVImplementationListChanged,
        IVAttributeChanged,
    };

    void setStorage(scs::SpaceCreatorProject *storage);
    scs::SpaceCreatorProject *storage() const;

    virtual bool isRefactorSupported(RefactorType type) const;

    // Querying function to check if refactoring is needed at all
    virtual bool isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const;
    virtual bool isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const;
    virtual bool isIVConnectionUsed(ivm::IVConnection *connection) const;

    // Functions to perform refactorings
    virtual void onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const;
    virtual void onIVInterfaceRenamed(
            ivm::IVInterface *interface, const QString &oldName, const QString &newName) const;
    virtual void onRemovingIVObject(ivm::IVObject *obj) const;

    // Changes regarding implementation (language)
    virtual void onImplementationChanged(ivm::IVFunction *entity, const QString &newName, const QString &oldName);
    virtual void onDefaultImplementationChanged();
    virtual void onImplementationListChanged(ivm::IVFunction *ivFunction);

    void onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);

protected:
    QPointer<scs::SpaceCreatorProject> m_storage;
};

} // namespace scs
