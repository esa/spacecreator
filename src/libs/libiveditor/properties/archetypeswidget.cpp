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

#include "archetypeswidgetmodel.h"
#include "archetype/comboboxdelegate.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "archetypes/archetypemodel.h"
#include "ui_archetypeswidget.h"

#include <QDebug>
#include <QHeaderView>

namespace ive {

ArchetypesWidget::ArchetypesWidget(ivm::ArchetypeModel *archetypeModel, ivm::IVFunctionType *function,
        ivm::AbstractSystemChecks *checks, shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArchetypesWidget)
{
    Q_ASSERT(function && function->model());
    ui->setupUi(this);

    ui->tableView->setItemDelegateForColumn(ArchetypesWidgetModel::Column::LibraryName,
            new shared::archetype::ComboBoxDelegate(archetypeModel->getLibrariesNames(), ui->tableView));
    ui->tableView->setItemDelegateForColumn(ArchetypesWidgetModel::Column::FunctionName,
            new shared::archetype::ComboBoxDelegate(QStringList(), ui->tableView));

    m_model = new ArchetypesWidgetModel(archetypeModel, checks, macro, this);
    m_model->setFunction(function);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->resizeSection(0, 220);
    ui->tableView->horizontalHeader()->resizeSection(1, 180);

    connect(ui->addButton, &QPushButton::clicked, this, &ArchetypesWidget::addArchetype);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ArchetypesWidget::deleteArchetype);
}

ArchetypesWidget::~ArchetypesWidget()
{
    delete ui;
}

void ArchetypesWidget::addArchetype()
{
    int newRow = m_model->rowCount();
    m_model->insertRow(newRow);
    QModelIndex idx = m_model->index(newRow, ArchetypesWidgetModel::Column::LibraryName);
    ui->tableView->edit(idx);
    ui->tableView->scrollToBottom();
    ui->tableView->selectRow(newRow);
}

void ArchetypesWidget::deleteArchetype()
{
    QModelIndexList selections = ui->tableView->selectionModel()->selectedRows();
    if (selections.size() != 1) {
        return;
    }

    m_model->removeRow(selections.at(0).row());
}

Ui::ArchetypesWidget *ArchetypesWidget::getUi()
{
    return ui;
}

} // namespace ive
