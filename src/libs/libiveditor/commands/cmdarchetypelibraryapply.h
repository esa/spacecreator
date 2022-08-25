/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "entityattribute.h"
#include "shared/common.h"
#include "undocommand.h"

#include <QHash>
#include <QPointer>
#include <QVector>

namespace ivm {
class IVModel;
class IVArchetypeLibraryReference;
class IVArchetypeReference;
}

namespace ive {
class InterfaceDocument;
namespace cmd {

class CmdArchetypeLibraryApply : public shared::UndoCommand
{
    Q_OBJECT
public:
    explicit CmdArchetypeLibraryApply(ive::InterfaceDocument *document, ivm::IVModel *objectsModel,
            QVector<ivm::IVArchetypeLibraryReference *> references);

    void redo() override;
    void undo() override;
    int id() const override;

private:
    void applyReferences(QHash<shared::Id, QVector<ivm::IVArchetypeReference *>> refencesForFunctions);
    bool isReferencePresent(const QString &libraryName);

private:
    QPointer<ivm::IVModel> m_model;
    QPointer<ive::InterfaceDocument> m_document;
    QVector<ivm::IVArchetypeLibraryReference *> m_newReferences;
    QVector<ivm::IVArchetypeLibraryReference *> m_oldReferences;
    QHash<shared::Id, QVector<ivm::IVArchetypeReference *>> m_newFunctionsArchetypeReferences;
    QHash<shared::Id, QVector<ivm::IVArchetypeReference *>> m_oldFunctionsArchetypeReferences;
};

} // namespace cmd
} // namespace ive
