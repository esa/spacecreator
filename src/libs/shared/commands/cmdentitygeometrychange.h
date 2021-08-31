/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "undocommand.h"

#include <QPointF>
#include <QPointer>
#include <QVector>

namespace shared {
class VEObject;
}

namespace shared {
namespace cmd {
class CmdEntityAutoLayout;

class CmdEntityGeometryChange : public UndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntityGeometryChange(
            const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData, const QString &title = {});
    ~CmdEntityGeometryChange() override;

    void redo() override;
    void undo() override;
    int id() const override;

    bool mergeGeometryData(const QList<QPair<VEObject *, QVector<QPointF>>> &objectsData);

protected:
    struct ObjectData {
        QPointer<shared::VEObject> entity;
        QVector<qint32> prevCoordinates;
        QVector<qint32> newCoordinates;

        bool operator==(const CmdEntityGeometryChange::ObjectData &data) const;
    };

    void prepareData(const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData);

private:
    static QList<ObjectData> convertData(const QList<QPair<shared::VEObject *, QVector<QPointF>>> &objectsData);

private:
    QList<ObjectData> m_data;
};

}
}
