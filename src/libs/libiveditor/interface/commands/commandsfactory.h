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

#include "commandids.h"

#include <QVariantList>

class QUndoCommand;

namespace ive {
namespace cmd {

class CommandsFactory
{
public:
    static QUndoCommand *create(Id id, const QVariantList &params);

private:
    static QUndoCommand *createFunctionCommand(const QVariantList &params);
    static QUndoCommand *createFunctionTypeCommand(const QVariantList &params);
    static QUndoCommand *createCommentCommand(const QVariantList &params);
    static QUndoCommand *createInterfaceCommand(const QVariantList &params);
    static QUndoCommand *createConnectionCommand(const QVariantList &params);
    static QUndoCommand *createConnectionGroupCommand(const QVariantList &params);
    static QUndoCommand *changeConnectionGroupCommand(const QVariantList &params);
    static QUndoCommand *changeGeometryCommand(const QVariantList &params);
    static QUndoCommand *removeEntityCommand(const QVariantList &params);
    static QUndoCommand *importEntitiesCommand(const QVariantList &params);
    static QUndoCommand *instantiateEntitiesCommand(const QVariantList &params);
    static QUndoCommand *addEntityPropertyCommand(const QVariantList &params);
    static QUndoCommand *changeEntityPropertyCommand(const QVariantList &params);
    static QUndoCommand *renameEntityPropertyCommand(const QVariantList &params);
    static QUndoCommand *removeEntityPropertyCommand(const QVariantList &params);
    static QUndoCommand *changeEntityAttributesCommand(const QVariantList &params);
    static QUndoCommand *createContextParameterCommand(const QVariantList &params);
    static QUndoCommand *changeContextParameterCommand(const QVariantList &params);
    static QUndoCommand *removeContextParameterCommand(const QVariantList &params);
    static QUndoCommand *createIfaceParamCommand(const QVariantList &params);
    static QUndoCommand *removeIfaceParamCommand(const QVariantList &params);
    static QUndoCommand *changeIfaceParamCommand(const QVariantList &params);
    static QUndoCommand *changeRiPropertyCommand(const QVariantList &params);
    static QUndoCommand *changeIfaceAttributeCommand(const QVariantList &params);
    static QUndoCommand *changeRootEntityCommand(const QVariantList &params);
    static QUndoCommand *autoLayoutEntityCommand(const QVariantList &params);
    static QUndoCommand *changeAsn1File(const QVariantList &params);

private:
    CommandsFactory() = delete;
    CommandsFactory(const CommandsFactory &other) = delete;
};

}
}
