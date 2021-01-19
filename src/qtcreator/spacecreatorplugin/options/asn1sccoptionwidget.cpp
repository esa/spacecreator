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

#include "asn1sccoptionwidget.h"

#include "asn1reader.h"
#include "ui_asn1sccoptionwidget.h"

#include <QFileDialog>

namespace spctr {

Asn1SccOptionWidget::Asn1SccOptionWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Asn1SccOptionWidget)
{
    ui->setupUi(this);

    Asn1Acn::Asn1Reader reader;
    ui->compilerLineEdit->setPlaceholderText(reader.checkforCompiler());
    ui->parameterLineEdit->setPlaceholderText(reader.defaultParameter());
    connect(ui->pathButton, &QPushButton::clicked, this, [this]() {
        QFileInfo fi(compiler());
        QString exec = QFileDialog::getOpenFileName(this, tr("Select asn1.scc.exe compiler"), fi.absolutePath());
        if (!exec.isEmpty()) {
            ui->compilerLineEdit->setText(exec);
        }
    });
}

Asn1SccOptionWidget::~Asn1SccOptionWidget()
{
    delete ui;
}

void Asn1SccOptionWidget::setCompiler(const QString &compiler)
{
    ui->compilerLineEdit->setText(compiler);
}

QString Asn1SccOptionWidget::compiler() const
{
    return ui->compilerLineEdit->text();
}

void Asn1SccOptionWidget::setParameter(const QString &parameter)
{
    ui->parameterLineEdit->setText(parameter);
}

QString Asn1SccOptionWidget::parameter() const
{
    return ui->parameterLineEdit->text();
}

}
