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

#include "commandsstack.h"
#include "ivinterface.h"
#include "shared/parameter.h"

#include <QVector>
#include <QWidget>
#include <memory>

namespace ivm {
class AbstractSystemChecks;
class IVFunctionType;
class ArchetypeModel;
class InterfaceArchetype;
class ParameterArchetype;
}

namespace Ui {
class ArchetypesWidget;
}

namespace ive {
class ArchetypesWidgetModel;

/*!
   UI to select the archetype(s) for functions
 */
class ArchetypesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArchetypesWidget(ivm::ArchetypeModel *archetypeModel, ivm::IVModel *layersModel, QPointer<Asn1Acn::Asn1SystemChecks> asn1Checks,
            ivm::IVFunctionType *function, cmd::CommandsStack::Macro *macro, QWidget *parent = nullptr);
    ~ArchetypesWidget();

    void addArchetype();
    void deleteArchetype();
    void applyArchetypes();

private:
    enum AsnCommentParseTokens
    {
        AsnParseFullLine = 0,
        AsnParsePreComment,
        AsnParsePostComment,
        AsnParseNoComment
    };

    bool checkReferences();
    ivm::IVInterface::CreationInfo generateCreationInfo(ivm::InterfaceArchetype *interfaceArchetype);
    QVector<shared::InterfaceParameter> generateInterfaceParameters(QVector<ivm::ParameterArchetype *> parameters);
    void rowsInserted(const QModelIndex &parent, int first, int last);

    bool handleMissingTypeNames();
    QStringList findMissingTypeNames();
    QString buildDefinitionsString(const QStringList& missingTypeNames);
    bool writeDefinitions(const QString& fileName, const QString& definitionsString);

private:
    std::unique_ptr<Ui::ArchetypesWidget> m_ui;
    QPointer<ivm::IVModel> m_layersModel;
    QPointer<Asn1Acn::Asn1SystemChecks> m_asn1Checks;
    QPointer<ArchetypesWidgetModel> m_model;
    QPointer<ivm::ArchetypeModel> m_archetypeModel;
    shared::cmd::CommandsStackBase::Macro *m_cmdMacro = nullptr;
};

} // namespace ive
