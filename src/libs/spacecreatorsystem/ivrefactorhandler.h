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

#include "ivobject.h"
#include "undocommand.h"

#include <QList>
#include <QObject>

namespace ive {
namespace cmd {
class CmdFunctionAttrChange;
class CmdIfaceAttrChange;
}
}
namespace ivm {
class IVFunction;
class IVInterface;
class IVObject;
}

namespace scs {

class RefactorBase;

/**
 * Class that receives events that might need refactor actions. And delegates them to the refactoring clients
 */
class IVRefactorHandler : public QObject
{
    Q_OBJECT
public:
    explicit IVRefactorHandler(QObject *parent = nullptr);

    void registerRefactor(RefactorBase *refactor);

public Q_SLOTS:
    void onIVEntityNameChanged(ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command);
    void onEntitiesRemoved(const QList<QPointer<ivm::IVObject>> &entities, shared::UndoCommand *command);
    void onImplementationChanged(
            ivm::IVFunction *entity, const QString &newName, const QString &oldName, shared::UndoCommand *command);
    void onDefaultImplementationChanged();
    void onImplementationListChanged(ivm::IVFunction *ivFunction);
    void onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);

private:
    void onIVFunctionNameChanged(
            ivm::IVFunction *func, const QString &oldName, ive::cmd::CmdFunctionAttrChange *command);
    void onIVInterfaceNameChanged(
            ivm::IVInterface *interface, const QString &oldName, ive::cmd::CmdIfaceAttrChange *command);

    void renameImplementationDirectory(const QString &oldName, const QString &newName);
    QString implementationBasePath() const;

    QList<RefactorBase *> m_refactors;
};

} // namespace scs
