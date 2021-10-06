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

#include "languageselect.h"

#include "interface/comboboxdelegate.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "languagemodel.h"
#include "ui_languageselect.h"

#include <QDebug>
#include <QHeaderView>

namespace ive {

LanguageSelect::LanguageSelect(ivm::IVFunction *fn, ivm::AbstractSystemChecks *checks,
        shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LanguageSelect)
{
    Q_ASSERT(fn && fn->model());
    ui->setupUi(this);

    ui->tableView->setItemDelegateForColumn(LanguageModel::Column::Language,
            new shared::ComboBoxDelegate(fn->model()->availableFunctionLanguages(), ui->tableView));

    m_model = new LanguageModel(checks, macro, this);
    m_model->setFunction(fn);
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->resizeSection(0, 220);
    ui->tableView->horizontalHeader()->resizeSection(1, 180);

    connect(ui->addButton, &QPushButton::clicked, this, &LanguageSelect::addLanguage);
    connect(ui->deleteButton, &QPushButton::clicked, this, &LanguageSelect::deleteSelectedLanguage);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &LanguageSelect::updateDeleteButton);
    updateDeleteButton();
}

LanguageSelect::~LanguageSelect()
{
    delete ui;
}

void LanguageSelect::addLanguage()
{
    int newRow = m_model->rowCount();
    m_model->insertRow(newRow);
    QModelIndex idx = m_model->index(newRow, LanguageModel::Column::Name);
    ui->tableView->edit(idx);
    ui->tableView->scrollToBottom();
}

void LanguageSelect::deleteSelectedLanguage()
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

void LanguageSelect::updateDeleteButton()
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

} // namespace ive
