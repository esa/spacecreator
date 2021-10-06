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

#pragma once

#include "dveditorcore.h"
#include "ivcore/abstractsystemchecks.h"

#include <QObject>
#include <QPointer>

namespace dvm {
class DVFunction;
class DVNode;
class DVSystemFunction;
}
namespace ivm {
class IVFunction;
class IVInterface;
}

namespace scs {
class SpaceCreatorProject;

class DvSystemChecks : public ivm::AbstractSystemChecks
{
    Q_OBJECT
public:
    explicit DvSystemChecks(QObject *parent = nullptr);

    // Setup functions
    void setStorage(scs::SpaceCreatorProject *storage);

public Q_SLOTS:
    bool checkDVFile(DVEditorCorePtr dvCore) const;

    bool checkFunctionBindings() const;
    bool checkFunctionBindings(DVEditorCorePtr dvCore) const;

    bool checkMessageBindings() const;
    bool checkMessageBindings(DVEditorCorePtr dvCore) const;

    ivm::IVFunction *correspondingFunction(dvm::DVFunction *dvFunc) const;
    dvm::DVFunction *correspondingFunction(ivm::IVFunction *ivFunc, const DVEditorCorePtr &dvCore) const;

    bool isImplementationUsed(ivm::IVFunction *ivFunc, const QString &name) const override;

private:
    bool checkFunctionIvValidity(const DVEditorCorePtr &dvCore) const;
    bool checkUniqueFunctionBindings(const DVEditorCorePtr &dvCore) const;
    bool checkSystemFunctionsAvailable(const DVEditorCorePtr &dvCore) const;
    bool checkSystemFunctionsAvailable(const dvm::DVNode *node, const DVEditorCorePtr &dvCore) const;
    bool checkProtectedFunctions(const DVEditorCorePtr &dvCore) const;

    bool checkMessageIvValidity(const DVEditorCorePtr &dvCore) const;
    bool checkUniqueMessages(const DVEditorCorePtr &dvCore) const;

    QPointer<SpaceCreatorProject> m_storage;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

} // namespace scs
