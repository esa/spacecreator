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

#include "refactorbase.h"

namespace dvm {
class DVMessage;
}

namespace scs {

/*!
 * Performs refactoring actions on DV data, triggered by other events
 */
class DVRefactor : public RefactorBase
{
public:
    bool isRefactorSupported(RefactorType type) const override;

    bool isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const override;
    bool isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const override;
    bool isIVConnectionUsed(ivm::IVConnection *connection) const override;

    void onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const override;
    void onIVInterfaceRenamed(
            ivm::IVInterface *interface, const QString &oldName, const QString &newName) const override;
    void onRemovingIVObject(ivm::IVObject *obj) const override;

    void onImplementationChanged(ivm::IVFunction *entity, const QString &newName, const QString &oldName) override;
    void onDefaultImplementationChanged() override;
    void onImplementationListChanged(ivm::IVFunction *ivFunction) override;

    void onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);

private:
    QList<dvm::DVMessage *> correspondingMessages(ivm::IVInterface *interface, const QString &name) const;
};

} // namespace scs
