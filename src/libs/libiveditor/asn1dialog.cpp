/*
 Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "asn1dialog.h"

#include "ui_asn1dialog.h"

#include <QFileInfo>

namespace ive {

Asn1Dialog::Asn1Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Asn1Dialog)
{
    ui->setupUi(this);
}

Asn1Dialog::~Asn1Dialog()
{
    delete ui;
}

void Asn1Dialog::setFile(const QFileInfo &file)
{
    ui->asnWidget->setFile(file);
}

QString Asn1Dialog::fileName() const
{
    return ui->asnWidget->fileName();
}

}
