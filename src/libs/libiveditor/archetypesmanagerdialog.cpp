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

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ArchetypesManagerDialog::~ArchetypesManagerDialog() {}

void ArchetypesManagerDialog::done(int r)
{
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
    m_ui->tabWidget->setCurrentIndex(0);

    if (!m_cmdMacro) {
        m_cmdMacro = std::make_unique<cmd::CommandsStack::Macro>(
                m_commandsStack.data(), tr("Archetypes library managment"));
    }
}

} // namespace ive
