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

#include <QPointF>
#include <QPointer>
#include <QUndoCommand>
#include <QVector>

namespace ivm {
class AADLObject;
}

namespace ive {
namespace cmd {

class CmdEntityGeometryChange : public QUndoCommand
{

public:
    explicit CmdEntityGeometryChange(
            const QList<QPair<ivm::AADLObject *, QVector<QPointF>>> &objectsData, const QString &title = {});
    ~CmdEntityGeometryChange() override;

    void redo() override;
    void undo() override;
    int id() const override;

    void mergeCommand(QUndoCommand *command);

protected:
    struct ObjectData {
        QPointer<ivm::AADLObject> entity;
        QVector<qint32> prevCoordinates;
        QVector<qint32> newCoordinates;
    };

    void prepareData(const QList<QPair<ivm::AADLObject *, QVector<QPointF>>> &objectsData);

private:
    static QList<ObjectData> convertData(const QList<QPair<ivm::AADLObject *, QVector<QPointF>>> &objectsData);

private:
    QList<QPair<ivm::AADLObject *, QVector<QPointF>>> m_internalData;
    QList<ObjectData> m_data;
    QList<QUndoCommand *> m_mergedCmds;
};

}
}
