/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "implementationswidget.h"

#include "implementationsmodel.h"
#include "interface/comboboxdelegate.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "ui_implementationswidget.h"

#include <QDebug>
#include <QHeaderView>

namespace ive {

ImplementationsWidget::ImplementationsWidget(const QString &projectPath, ivm::IVFunction *fn,
        ivm::AbstractSystemChecks *checks, shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImplementationsWidget)
{
    Q_ASSERT(fn && fn->model());
    ui->setupUi(this);

    ui->tableView->setItemDelegateForColumn(ImplementationsModel::Column::Language,
            new shared::ComboBoxDelegate(fn->model()->availableFunctionLanguages(), ui->tableView));

    m_model = new ImplementationsModel(projectPath, checks, macro, this);
    m_model->setFunction(fn);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->resizeSection(0, 220);
    ui->tableView->horizontalHeader()->resizeSection(1, 180);

    connect(ui->addButton, &QPushButton::clicked, this, &ImplementationsWidget::addLanguage);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ImplementationsWidget::deleteSelectedLanguage);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &ImplementationsWidget::updateDeleteButton);
    updateDeleteButton();

    connect(m_model, &ImplementationsModel::rowsInserted, this, &ImplementationsWidget::rowsInserted);
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);
}

ImplementationsWidget::~ImplementationsWidget()
{
    delete ui;
}

void ImplementationsWidget::addLanguage()
{
    int newRow = m_model->rowCount();
    m_model->insertRow(newRow);
    QModelIndex idx = m_model->index(newRow, ImplementationsModel::Column::Name);
    ui->tableView->edit(idx);
    ui->tableView->scrollToBottom();
    ui->tableView->selectRow(newRow);
}

void ImplementationsWidget::deleteSelectedLanguage()
{
    QModelIndexList selections = ui->tableView->selectionModel()->selectedRows();
    if (selections.size() != 1) {
        return;
    }
    if (selections.at(0).row() == m_model->defaultIndex().row()) {
        return;
    }

    m_model->removeRow(selections.at(0).row());
}

void ImplementationsWidget::updateDeleteButton()
{
    QModelIndexList selections = ui->tableView->selectionModel()->selectedRows();
    bool editable = true;
    if (selections.size() != 1) {
        editable = false;
    } else {
        int selectedRow = selections.at(0).row();
        if (selectedRow == m_model->defaultIndex().row() || m_model->isImplementationUsed(selectedRow)) {
            editable = false;
        }
    }

    ui->deleteButton->setEnabled(editable);
}

void ImplementationsWidget::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int idx = first; idx <= last; ++idx) {
        const QModelIndex index = m_model->index(idx, ImplementationsModel::Column::Language, parent);
        if (index.isValid()) {
            ui->tableView->openPersistentEditor(index);
        }
    }
}

} // namespace ive
