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

#include <QPointer>
#include <QUndoCommand>
#include <QVector>

class QTemporaryDir;
namespace ivm {
class IVObject;
class IVFunctionType;
class IVModel;
}

namespace ive {
namespace cmd {

class CmdEntitiesImport : public QUndoCommand
{
public:
    explicit CmdEntitiesImport(const QByteArray &data, ivm::IVFunctionType *parent, ivm::IVModel *model,
            const QPointF &pos, const QString &destPath);
    ~CmdEntitiesImport() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    void redoSourceCloning(const ivm::IVObject *object);
    void undoSourceCloning(const ivm::IVObject *object);
    QString relativePathForObject(const ivm::IVObject *object) const;

    QPointer<ivm::IVModel> m_model;
    QPointer<ivm::IVModel> m_importModel;
    QPointer<ivm::IVFunctionType> m_parent;
    QVector<QPointer<ivm::IVObject>> m_rootEntities;
    QVector<QPointer<ivm::IVObject>> m_importedEntities;

    QString m_destPath;
    QScopedPointer<QTemporaryDir> m_tempDir;
};

} // namespace ive
} // namespace cmd
