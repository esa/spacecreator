/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "addnewrequirementdialog.h"

#include "ui_addnewrequirementdialog.h"

#include <QDialogButtonBox>
#include <QPushButton>

namespace requirement {

AddNewRequirementDialog::AddNewRequirementDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddNewRequirementDialog)
{
    ui->setupUi(this);
    ui->gridLayout->setVerticalSpacing(10);

    connect(ui->titleLineEdit, &QLineEdit::textChanged, this, &AddNewRequirementDialog::updateOkButton);
    connect(ui->idLineEdit, &QLineEdit::textChanged, this, &AddNewRequirementDialog::updateOkButton);
    updateOkButton();
}

AddNewRequirementDialog::~AddNewRequirementDialog()
{
    delete ui;
}

/*!
 * Returns the title of the requirements.
 * A title is mandatory to add a requirement
 */
QString AddNewRequirementDialog::title() const
{
    return ui->titleLineEdit->text();
}

QString AddNewRequirementDialog::description() const
{
    return ui->descriptionTextEdit->toPlainText();
}

/*!
 * Returns the ID of the requirements.
 * An ID is mandatory to add a requirement
 */
QString AddNewRequirementDialog::reqIfId() const
{
    return ui->idLineEdit->text();
}

void AddNewRequirementDialog::updateOkButton()
{
    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (!okButton) {
        return;
    }

    okButton->setEnabled(!ui->titleLineEdit->text().isEmpty() && !ui->idLineEdit->text().isEmpty());
}

}
