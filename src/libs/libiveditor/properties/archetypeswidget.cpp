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

#include "archetypeswidget.h"

#include "archetypes/archetypemodel.h"
#include "archetypes/archetypeobject.h"
#include "archetypes/functionarchetype.h"
#include "archetypes/interfacearchetype.h"
#include "archetypes/parameterarchetype.h"
#include "archetypeswidgetmodel.h"
#include "commands/cmdfunctionarchetypesapply.h"
#include "commands/cmdinterfaceitemcreate.h"
#include "delegates/comboboxdelegate.h"
#include "ivarchetypereference.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ui_archetypeswidget.h"

#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

namespace ive {

ArchetypesWidget::ArchetypesWidget(ivm::ArchetypeModel *archetypeModel, ivm::IVFunctionType *function,
        ivm::AbstractSystemChecks *checks, shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ArchetypesWidget)
    , m_archetypeModel(archetypeModel)
    , m_cmdMacro(macro)
{
    Q_ASSERT(function && function->model());
    m_ui->setupUi(this);

    m_ui->tableView->setItemDelegateForColumn(ArchetypesWidgetModel::Column::LibraryName,
            new ive::ComboBoxDelegate(archetypeModel->getLibrariesNames(), m_ui->tableView));
    m_ui->tableView->setItemDelegateForColumn(
            ArchetypesWidgetModel::Column::FunctionName, new ive::ComboBoxDelegate(QStringList(), m_ui->tableView));

    m_model = new ArchetypesWidgetModel(archetypeModel, checks, macro, this);
    m_model->setFunction(function);
    m_ui->tableView->setModel(m_model);
    m_ui->tableView->horizontalHeader()->resizeSection(0, 220);
    m_ui->tableView->horizontalHeader()->resizeSection(1, 180);

    connect(m_ui->addButton, &QPushButton::clicked, this, &ArchetypesWidget::addArchetype);
    connect(m_ui->deleteButton, &QPushButton::clicked, this, &ArchetypesWidget::deleteArchetype);

    connect(m_model, &ArchetypesWidgetModel::rowsInserted, this, &ArchetypesWidget::rowsInserted);
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);
}

ArchetypesWidget::~ArchetypesWidget()
{
    delete m_ui;
}

void ArchetypesWidget::addArchetype()
{
    int newRow = m_model->rowCount();

    if (!m_model->insertRow(newRow)) {
        return;
    }

    QModelIndex idx = m_model->index(newRow, ArchetypesWidgetModel::Column::LibraryName);
    m_ui->tableView->edit(idx);
    m_ui->tableView->scrollToBottom();
    m_ui->tableView->selectRow(newRow);
}

void ArchetypesWidget::deleteArchetype()
{
    QModelIndexList selections = m_ui->tableView->selectionModel()->selectedRows();
    if (selections.size() != 1) {
        return;
    }

    m_model->removeRow(selections.at(0).row());
}

void ArchetypesWidget::applyArchetypes()
{
    if (!checkReferences() || !m_model->areArchetypesModified()) {
        return;
    }

    auto command = new cmd::CmdFunctionArchetypesApply(m_model->getFunction(), m_model->getArchetypeReferences());
    m_cmdMacro->push(command);

    for (auto reference : m_model->getArchetypeReferences()) {

        ivm::ArchetypeObject *archetypeObject = m_archetypeModel->getObjectByName(
                reference->getFunctionName(), ivm::ArchetypeObject::Type::FunctionArchetype);
        ivm::FunctionArchetype *functionArchetype = archetypeObject->as<ivm::FunctionArchetype *>();

        if (functionArchetype == nullptr) {
            continue;
        }

        for (auto interface : functionArchetype->getInterfaces()) {
            if (m_model->getFunction()->hasInterface(interface->title())) {
                QMessageBox::warning(qApp->activeWindow(), tr("Duplicate interface"),
                        tr("Function %1 already has an interface named %2")
                                .arg(m_model->getFunction()->title())
                                .arg(interface->title()));
                continue;
            }

            ivm::IVInterface::CreationInfo creationInfo = generateCreationInfo(interface);

            auto command = new cmd::CmdInterfaceItemCreate(creationInfo);
            m_cmdMacro->push(command);
        }
    }
}

bool ArchetypesWidget::checkReferences()
{
    QVector<ivm::IVArchetypeReference *> references = m_model->getArchetypeReferences();

    for (auto reference : references) {
        if (reference->getLibraryName().isEmpty() || reference->getFunctionName().isEmpty()) {
            QMessageBox::warning(
                    qApp->activeWindow(), tr("Archetype error"), tr("Archetypes implementations are incomplete"));
            return false;
        }
    }

    for (int i = 0; i < references.size(); ++i) {
        for (int j = i + 1; j < references.size(); ++j) {
            if (references[i]->getLibraryName() == references[j]->getLibraryName()
                    && references[i]->getFunctionName() == references[j]->getFunctionName()) {
                QMessageBox::warning(qApp->activeWindow(), tr("Archetype error"),
                        tr("Archetype list has duplicate implementations"));
                return false;
            }
        }
    }

    return true;
}

ivm::IVInterface::CreationInfo ArchetypesWidget::generateCreationInfo(ivm::InterfaceArchetype *interfaceArchetype)
{
    ivm::IVInterface::CreationInfo creationInfo;

    switch (interfaceArchetype->getInterfaceType()) {
    case ivm::InterfaceArchetype::InterfaceType::PROVIDED:
        creationInfo.type = ivm::IVInterface::InterfaceType::Provided;
        break;
    case ivm::InterfaceArchetype::InterfaceType::REQUIRED:
        creationInfo.type = ivm::IVInterface::InterfaceType::Required;
        break;
    }

    switch (interfaceArchetype->getOperationKind()) {
    case ivm::InterfaceArchetype::OperationKind::CYCLIC:
        creationInfo.kind = ivm::IVInterface::OperationKind::Cyclic;
        break;
    case ivm::InterfaceArchetype::OperationKind::SPORADIC:
        creationInfo.kind = ivm::IVInterface::OperationKind::Sporadic;
        break;
    case ivm::InterfaceArchetype::OperationKind::PROTECTED:
        creationInfo.kind = ivm::IVInterface::OperationKind::Protected;
        break;
    case ivm::InterfaceArchetype::OperationKind::UNPROTECTED:
        creationInfo.kind = ivm::IVInterface::OperationKind::Unprotected;
        break;
    }

    ivm::IVConnectionLayerType *layer = new ivm::IVConnectionLayerType();
    layer->rename(interfaceArchetype->getLayer());

    creationInfo.parameters = generateInterfaceParameters(interfaceArchetype->getParameters());
    creationInfo.layer = layer;
    creationInfo.function = m_model->getFunction();
    creationInfo.model = m_model->getFunction()->model();
    creationInfo.name = interfaceArchetype->title();

    return creationInfo;
}

QVector<shared::InterfaceParameter> ArchetypesWidget::generateInterfaceParameters(
        QVector<ivm::ParameterArchetype *> parameters)
{
    QVector<shared::InterfaceParameter> resultParameters;

    for (auto archetypeParameter : parameters) {
        shared::InterfaceParameter::Direction parameterDirection;
        switch (archetypeParameter->getDirection()) {
        case ivm::ParameterArchetype::ParameterDirection::IN:
            parameterDirection = shared::InterfaceParameter::Direction::IN;
            break;
        case ivm::ParameterArchetype::ParameterDirection::OUT:
            parameterDirection = shared::InterfaceParameter::Direction::OUT;
            break;
        }

        shared::InterfaceParameter parameter(archetypeParameter->title(), shared::BasicParameter::Type::Other,
                archetypeParameter->getType(), QObject::tr("NATIVE"), parameterDirection);

        resultParameters.append(parameter);
    }

    return resultParameters;
}

void ArchetypesWidget::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int i = first; i <= last; ++i) {
        const QModelIndex functionIndex = m_model->index(i, ArchetypesWidgetModel::Column::FunctionName, parent);
        if (functionIndex.isValid()) {
            m_ui->tableView->openPersistentEditor(functionIndex);
        }
    }
}

} // namespace ive
