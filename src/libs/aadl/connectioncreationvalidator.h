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

#include "common.h"

#include <QObject>
#include <QPointF>
#include <QVector>

class QGraphicsScene;
class QGraphicsItem;

namespace ivm {

class AADLObjectIface;
class AADLObjectFunction;
class ConnectionCreationValidator
{
    Q_GADGET

public:
    enum class FailReason
    {
        NotFail = 0,
        IsFunctionType,
        MulticastDisabled,
        KindDiffer,
        ParamsDiffer,
        ParentIsFunctionType,
        IsCyclic,
        SameParent,
        SameInterface,
        NoScene,
        NoStartFunction,
        CannotCreateStartIface,
        NoEndFunction,
        CannotCreateEndIface,
        SameDirectionIfaceWrongParents,
        ToFromNestedDifferentDirection,
        DirectIfaceCreationInInstanceOfFunctionType,
    };
    Q_ENUM(FailReason)

    static ConnectionCreationValidator::FailReason canConnect(AADLObjectFunction *sourceFunction,
            AADLObjectFunction *targetFunction, AADLObjectIface *sourceIource, AADLObjectIface *targetIface);

private:
    ConnectionCreationValidator();

    static ConnectionCreationValidator::FailReason checkKindAndParams(
            AADLObjectIface *sourceIface, AADLObjectIface *targetIface);
};

struct ValidationResult {
    ivm::AADLObjectIface *startIface { nullptr };
    ivm::AADLObjectIface *endIface { nullptr };
    shared::Id startIfaceId = {};
    shared::Id endIfaceId = {};
    QPointF startPointAdjusted {};
    QPointF endPointAdjusted {};
    QVector<QPointF> connectionPoints;
    QGraphicsItem *functionAtStartPos { nullptr };
    ivm::AADLObjectFunction *startObject { nullptr };
    QGraphicsItem *functionAtEndPos { nullptr };
    ivm::AADLObjectFunction *endObject { nullptr };
    bool isToOrFromNested { false };

    ConnectionCreationValidator::FailReason status { ConnectionCreationValidator::FailReason::NoScene };
    inline bool failed() const { return status != ConnectionCreationValidator::FailReason::NotFail; }
    inline bool setFailed(ConnectionCreationValidator::FailReason s)
    {
        status = s;
        return failed();
    }
};

}
