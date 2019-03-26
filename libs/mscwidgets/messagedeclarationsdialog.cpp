/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "messagedeclarationsdialog.h"

#include "mscdocument.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "ui_messagedeclarationsdialog.h"

#include <QDebug>
#include <QItemSelectionModel>

MessageDeclarationsDialog::MessageDeclarationsDialog(msc::MscMessageDeclarationList *model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageDeclarationsDialog)
    , m_model(model->clone())
{
    Q_ASSERT(model);
    ui->setupUi(this);

    ui->messagesView->setModel(m_model);

    connect(ui->messagesView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &MessageDeclarationsDialog::showSelectedMessage);
    connect(ui->addDeclarationButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::addDeclaration);
    connect(ui->removeDeclarationButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::removeDeclaration);

    connect(ui->nameLineEdit, &QLineEdit::editingFinished, this, &MessageDeclarationsDialog::updateNames);
}

MessageDeclarationsDialog::~MessageDeclarationsDialog()
{
    delete ui;
    m_model->clear(true);
    delete m_model;
}

msc::MscMessageDeclarationList *MessageDeclarationsDialog::declarations() const
{
    return m_model;
}

void MessageDeclarationsDialog::showSelectedMessage()
{
    QItemSelectionModel *selectionModel = ui->messagesView->selectionModel();
    if (!selectionModel->hasSelection()) {
        ui->nameLineEdit->clear();
        ui->nameLineEdit->setEnabled(false);
        m_selectedDeclaration = nullptr;
        ui->removeDeclarationButton->setEnabled(false);
        return;
    }

    m_selectedDeclaration = m_model->at(selectionModel->selectedIndexes().at(0).row());
    ui->nameLineEdit->setText(m_selectedDeclaration->names().join(", "));
    ui->nameLineEdit->setEnabled(true);
    ui->removeDeclarationButton->setEnabled(true);
}

void MessageDeclarationsDialog::addDeclaration()
{
    auto declaration = new msc::MscMessageDeclaration(m_model);
    declaration->setNames({ tr("message_name") });
    m_model->append(declaration);

    QModelIndex index = m_model->index(m_model->indexOf(declaration));
    ui->messagesView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    ui->nameLineEdit->setFocus();
}

void MessageDeclarationsDialog::removeDeclaration()
{
    if (!m_selectedDeclaration)
        return;

    int idx = m_model->indexOf(m_selectedDeclaration);
    m_model->removeAt(idx);
    idx = std::min(m_model->size() - 1, idx);
    if (idx >= 0) {
        QModelIndex index = m_model->index(idx);
        ui->messagesView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    } else {
        ui->messagesView->clearSelection();
    }
}

void MessageDeclarationsDialog::updateNames()
{
    if (!m_selectedDeclaration)
        return;

    QStringList names = ui->nameLineEdit->text().split(",");
    for (QString &name : names) {
        name = name.trimmed();
    }

    m_selectedDeclaration->setNames(names);
}
