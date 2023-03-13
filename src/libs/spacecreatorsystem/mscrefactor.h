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

#include <QList>
#include <QPointer>

namespace msc {
class MscMessage;
}

namespace scs {
class MscSystemChecks;
class SpaceCreatorProject;

/*!
 * Performs refactoring actions on MSC data, triggered by other events
 */
class MSCRefactor : public RefactorBase
{
public:
    void setStorage(SpaceCreatorProject *storage);
    void setMscChecks(MscSystemChecks *mscChecks);

    bool isRefactorSupported(RefactorType type) const override;

    bool isIVFunctionUsed(ivm::IVFunction *func, const QString &name) const override;
    bool isIVInterfaceUsed(ivm::IVInterface *interface, const QString &name) const override;
    bool isIVConnectionUsed(ivm::IVConnection *connection) const override;

    void onIVFunctionRenamed(ivm::IVFunction *func, const QString &oldName, const QString &newName) const override;
    void onIVInterfaceRenamed(
            ivm::IVInterface *interface, const QString &oldName, const QString &newName) const override;
    void onRemovingIVObject(ivm::IVObject *obj) const override;

private:
    QList<msc::MscMessage *> correspondingMessages(ivm::IVInterface *interface, const QString &name) const;

    QPointer<SpaceCreatorProject> m_storage;
    QPointer<MscSystemChecks> m_mscChecks;
};

} // namespace scs
