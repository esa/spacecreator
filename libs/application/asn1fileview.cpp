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

#include "asn1fileview.h"

#include "asn1xmlparser.h"
#include "commands/common/commandsstack.h"
#include "mscmodel.h"
#include "ui_asn1fileview.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>

ASN1FileView::ASN1FileView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ASN1FileView)
{
    ui->setupUi(this);
    ui->textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    connect(ui->selectFileButton, &QPushButton::clicked, this, &ASN1FileView::selectFile);
}

ASN1FileView::~ASN1FileView()
{
    delete ui;
}

void ASN1FileView::setModel(msc::MscModel *model)
{
    if (model == m_model)
        return;

    m_model = model;
    if (m_model)
        connect(m_model, &msc::MscModel::dataDefinitionStringChanged, this, &ASN1FileView::updateView);

    updateView();
}

void ASN1FileView::setCurrentDirectory(const QString &directory)
{
    m_currentDirectory = directory;
    updateView();
}

void ASN1FileView::updateView()
{
    ui->filenameLabel->clear();
    ui->textEdit->clear();

    if (!m_model)
        return;

    ui->filenameLabel->setText(m_model->dataDefinitionString());
    fillPreview();
}

void ASN1FileView::selectFile()
{
    if (!m_model)
        return;

    QString file =
            QFileDialog::getOpenFileName(this, tr("ASN.1 file"), QString(), QStringLiteral("ASN1 files (*.asn *.ASN)"));
    if (!file.isEmpty()) {
        QFileInfo fi(file);
        const QVariantList params { QVariant::fromValue(m_model.data()), fi.fileName(), "ASN.1" };
        msc::cmd::CommandsStack::push(msc::cmd::Id::SetAsn1File, params);
        fillPreview(file);
    }
}

void ASN1FileView::fillPreview()
{
    if (!m_model->dataDefinitionString().isEmpty()) {
        QString filename = m_currentDirectory + QDir::separator() + m_model->dataDefinitionString();
        fillPreview(filename);
    } else
        ui->textEdit->clear();
}

void ASN1FileView::fillPreview(const QString &filename)
{
    QFileInfo fi(filename);
    if (!fi.exists())
        qWarning() << "Can't find file" << filename;

    asn1::Asn1XMLParser parser;
    QString html = parser.asn1AsHtml(filename);
    if (html.isEmpty()) {
        QFile file(filename);
        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly))
                ui->textEdit->setText(file.readAll());
        }
    } else {
        ui->textEdit->setHtml(html);
    }
}
