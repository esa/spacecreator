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

#include "archetypesmanagerdialog.h"

#include "archetypesmanagermodel.h"
#include "commands/cmdarchetypelibraryapply.h"
#include "interfacedocument.h"
#include "ivarchetypelibraryreference.h"
#include "ivmodel.h"
#include "veobject.h"
#include "properties/delegates/filedialogdelegate.h"
#include "shared/common.h"
#include "ui_archetypesmanagerdialog.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QPointer>
#include <QTableView>
#include <QTimer>
#include <QUndoCommand>

namespace ive {

ArchetypesManagerDialog::ArchetypesManagerDialog(ive::InterfaceDocument *document, ivm::IVModel *objectsModel,
        cmd::CommandsStack *commandsStack, QWidget *parent)
    : QDialog(parent)
    , m_objectsModel(objectsModel)
    , m_document(document)
    , m_commandsStack(commandsStack)
    , m_ui(std::make_unique<Ui::ArchetypesManagerDialog>())
{
    m_ui->setupUi(this);

    m_ui->tableView->setItemDelegateForColumn(ArchetypesManagerModel::Column::LibraryPath,
            new ive::FileDialogDelegate(shared::interfaceCustomArchetypesDirectoryPath(),
                    tr("Chose archetype library file"), "*.xml", m_ui->tableView));

    m_model = new ArchetypesManagerModel(objectsModel, m_cmdMacro.get(), this);
    m_ui->tableView->setModel(m_model);

    m_ui->gridLayout_4->setColumnMinimumWidth(1, 15);
    m_ui->gridLayout_4->setColumnStretch(0, 10);
    m_ui->tableView->horizontalHeader()->resizeSection(0, 150);
    m_ui->tableView->horizontalHeader()->resizeSection(1, 220);

    connect(m_ui->addButton, &QPushButton::clicked, this, &ArchetypesManagerDialog::addArchetypeLibrary);
    connect(m_ui->deleteButton, &QPushButton::clicked, this, &ArchetypesManagerDialog::deleteArchetypeLibrary);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &ArchetypesManagerDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &ArchetypesManagerDialog::reject);
}

ArchetypesManagerDialog::~ArchetypesManagerDialog() {}

void ArchetypesManagerDialog::addArchetypeLibrary()
{
    int newRow = m_model->rowCount();

    if (!m_model->insertRow(newRow)) {
        return;
    }

    QModelIndex idx = m_model->index(newRow, ArchetypesManagerModel::Column::LibraryPath);
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

    m_ui->tableView->selectionModel()->clear();
    m_model->removeRow(selections.at(0).row());
}

void ArchetypesManagerDialog::accept()
{
    if (!checkReferences()) {
        return;
    }

    auto command =
            new cmd::CmdArchetypeLibraryApply(m_document, m_objectsModel, m_model->getArchetypeLibraryReferences());
    m_cmdMacro->push(command);

    m_cmdMacro->setComplete(true);
    QDialog::done(QDialog::Accepted);
}

void ArchetypesManagerDialog::reject()
{
    m_cmdMacro->setComplete(false);
    QDialog::done(QDialog::Rejected);
}

void ArchetypesManagerDialog::init()
{
    if (m_cmdMacro == nullptr) {
        m_cmdMacro =
                std::make_unique<cmd::CommandsStack::Macro>(m_commandsStack.data(), tr("Archetypes library managment"));
    }
}

bool ArchetypesManagerDialog::checkReferences()
{
    QVector<ivm::IVArchetypeLibraryReference *> references = m_model->getArchetypeLibraryReferences();

    for (auto reference : references) {
        if (reference->getLibraryName().isEmpty() || reference->getLibraryPath().isEmpty()) {
            QMessageBox::warning(
                    qApp->activeWindow(), tr("Archetype error"), tr("Archetypes library references are incomplete"));
            return false;
        }
    }

    if (shared::VEObject::hasDuplicates(references)) {
        QMessageBox::warning(
                qApp->activeWindow(), tr("Archetype error"), tr("Archetype library list has duplicate references"));
        return false;
    }

    return true;
}

} // namespace ive
