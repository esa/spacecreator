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

#include "asn1componentsimport.h"
#include "common.h"
#include "ivmodel.h"

#include <QPointer>
#include <QUndoCommand>
#include <QVector>

class QTemporaryDir;
namespace ivm {
class IVObject;
class IVFunctionType;
}

namespace ive {
namespace cmd {

class CmdEntitiesImport : public ASN1ComponentsImport, public QUndoCommand
{
    Q_OBJECT
public:
    explicit CmdEntitiesImport(const QList<ivm::IVObject *> &entities, ivm::IVFunctionType *parent, ivm::IVModel *model,
            Asn1Acn::Asn1SystemChecks *asn1Checks, const QPointF &pos, const QString &destPath);
    ~CmdEntitiesImport() override;

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    bool init(const QVector<ivm::IVObject *> &objects, const QPointF &pos, QString *errorString = nullptr);
    void prepareRectangularType(
            ivm::IVObject *obj, const QPointF &offset, QRectF &parentRect, QList<QRectF> &existingRects);
    void prepareEndPointType(ivm::IVObject *obj, const QPointF &offset);
    void prepareConnectionType(ivm::IVObject *obj, const QVector<ivm::IVObject *> &objects);
    QList<QRectF> existingModelRects() const;

private:
    QPointer<ivm::IVModel> m_model;
    QPointer<ivm::IVFunctionType> m_parent;
    QVector<QPointer<ivm::IVObject>> m_rootEntities;
    QVector<QPointer<ivm::IVObject>> m_importedEntities;
    QHash<QString, QString> m_renamedFunctions;
    QHash<shared::Id, QPointer<ivm::IVObject>> m_parentChildMappings;
};

} // namespace ive
} // namespace cmd
