/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstackbase.h"

#include <QObject>

class QUndoCommand;
class QUndoStack;

namespace ivm {
class IVFunction;
class IVObject;
}

namespace shared {
class UndoCommand;
class VEObject;
}

namespace ive {
namespace cmd {

class CommandsStack : public shared::cmd::CommandsStackBase
{
    Q_OBJECT
public:
    CommandsStack(QObject *parent = nullptr);

    bool push(QUndoCommand *command) override;

Q_SIGNALS:
    void nameChanged(ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command);
    void entitiesRemoved(const QList<QPointer<ivm::IVObject>> &entities, shared::UndoCommand *command);
    void implementationChanged(
            ivm::IVFunction *entity, const QString &newName, const QString &oldName, shared::UndoCommand *command);
    void defaultImplementationChanged(ivm::IVFunction *entity);
    void implementationListChanged(ivm::IVFunction *entity);
    void attributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);
    void asn1FilesImported(const QStringList &files);
    void asn1FileRemoved(const QStringList &files);
};

}
}
