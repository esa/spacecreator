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

#include "app/common.h"

#include <QLineF>
#include <QObject>
#include <QPointF>
#include <QVector>

class QGraphicsScene;
class QGraphicsItem;
namespace taste3 {
namespace aadl {

class AADLObjectIface;
class AADLObjectFunction;
class ConnectionCreationValidator
{
    Q_GADGET

public:
    static const qreal kInterfaceTolerance;
    static const qreal kConnectionTolerance;

    enum class FailReason
    {
        NotFail = 0,
        NoPreviewItem,
        IsFunctionType,
        MulticastDisabled,
        KindDiffer,
        ParamsDiffer,
        ParentIsFunctionType,
        IsCyclic,
        SameParent,
        NoScene,
        NoStartFunction,
        CannotCreateStartIface,
        NoEndFunction,
        CannotCreateEndIface,
        SameDirectionIfaceWrongParents,
    };
    Q_ENUM(FailReason)

    struct ValidationResult {
        AADLObjectIface *startIface { nullptr };
        AADLObjectIface *endIface { nullptr };
        common::Id startIfaceId = {};
        common::Id endIfaceId = {};
        QPointF startPointAdjusted {};
        QPointF endPointAdjusted {};
        QLineF connectionLine {};
        QGraphicsItem *functionAtStartPos { nullptr };
        AADLObjectFunction *startObject { nullptr };
        QGraphicsItem *functionAtEndPos { nullptr };
        AADLObjectFunction *endObject { nullptr };
        bool isToOrFromNested { false };

        FailReason status { FailReason::NoScene };
        inline bool failed() const { return status != ConnectionCreationValidator::FailReason::NotFail; }
        inline bool setFailed(ConnectionCreationValidator::FailReason s)
        {
            status = s;
            return failed();
        }
    };

    static ConnectionCreationValidator::ValidationResult validate(QGraphicsScene *scene,
                                                                  const QVector<QPointF> &connectionPoints);

    static ConnectionCreationValidator::FailReason canConnect(AADLObjectFunction *sourceFunction,
                                                              AADLObjectFunction *targetFunction,
                                                              AADLObjectIface *sourceIource,
                                                              AADLObjectIface *targetIface);

private:
    ConnectionCreationValidator();

    static ConnectionCreationValidator::ValidationResult validateCreate(QGraphicsScene *scene,
                                                                        const QVector<QPointF> &connectionPoints);
};

typedef ConnectionCreationValidator::ValidationResult ConnectionInfo;

} // namespace aadl
} // namespace taste3
