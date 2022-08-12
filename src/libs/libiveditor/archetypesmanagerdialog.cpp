/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypesmanagerdialog.h"

#include "archetypesmanagermodel.h"
#include "commands/cmdarchetypelibraryapply.h"
#include "ivmodel.h"
#include "ui_archetypesmanagerdialog.h"

#include <QDebug>
#include <QPointer>
#include <QTableView>
#include <QTimer>
#include <QUndoCommand>

namespace ive {

ArchetypesManagerDialog::ArchetypesManagerDialog(
        ivm::IVModel *objectsModel, cmd::CommandsStack *commandsStack, QWidget *parent)
    : QDialog(parent)
    , m_objectsModel(objectsModel)
    , m_commandsStack(commandsStack)
    , m_ui(new Ui::ArchetypesManagerDialog)
{
    m_ui->setupUi(this);

    m_model = new ArchetypesManagerModel(objectsModel, m_cmdMacro.get(), this);
    m_ui->tableView->setModel(m_model);

    m_ui->gridLayout_4->setColumnMinimumWidth(1, 15);
    m_ui->gridLayout_4->setColumnStretch(0, 10);
    m_ui->tableView->horizontalHeader()->resizeSection(0, 150);
    m_ui->tableView->horizontalHeader()->resizeSection(1, 220);

    connect(m_ui->addButton, &QPushButton::clicked, this, &ArchetypesManagerDialog::addArchetypeLibrary);
    connect(m_ui->deleteButton, &QPushButton::clicked, this, &ArchetypesManagerDialog::deleteArchetypeLibrary);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ArchetypesManagerDialog::~ArchetypesManagerDialog() {}

void ArchetypesManagerDialog::addArchetypeLibrary()
{
    int newRow = m_model->rowCount();

    if (!m_model->insertRow(newRow)) {
        return;
    }

    QModelIndex idx = m_model->index(newRow, ArchetypesManagerModel::Column::LibraryName);
    m_ui->tableView->edit(idx);
    m_ui->tableView->scrollToBottom();
    m_ui->tableView->selectRow(newRow);
}

void ArchetypesManagerDialog::deleteArchetypeLibrary()
{
    QModelIndexList selections = m_ui->tableView->selectionModel()->selectedRows();
    if (selections.size() != 1) {
        return;
    }

    m_model->removeRow(selections.at(0).row());
}

void ArchetypesManagerDialog::done(int r)
{
    auto command = new cmd::CmdArchetypeLibraryApply(m_objectsModel, m_model->getArchetypeLibraryReferences());
    m_cmdMacro->push(command);

    if (!m_cmdMacro) {
        r = QDialog::Rejected;
    } else {
        m_cmdMacro->setComplete(r == QDialog::Accepted);
    }

    QDialog::done(r);
}

void ArchetypesManagerDialog::init()
{
    setWindowTitle(tr("Archetypes Library Manager"));

    if (!m_cmdMacro) {
        m_cmdMacro =
                std::make_unique<cmd::CommandsStack::Macro>(m_commandsStack.data(), tr("Archetypes library managment"));
    }
}

} // namespace ive
