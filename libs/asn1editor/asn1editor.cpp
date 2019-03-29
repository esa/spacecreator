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

#include "asn1editor.h"

#include "asn1treeview.h"
#include "asn1valueparser.h"
#include "asn1xmlparser.h"
#include "ui_asn1editor.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

namespace asn1 {

Asn1Editor::Asn1Editor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Asn1Editor)
{
    ui->setupUi(this);

    m_asn1TreeView = new Asn1TreeView(this);

    ui->verticalLayout->insertWidget(1, m_asn1TreeView);

    connect(ui->openBtn, &QPushButton::clicked, this, &Asn1Editor::openFile);
    connect(ui->typesCB, &QComboBox::currentTextChanged, this, &Asn1Editor::showAsn1Type);
    connect(ui->valueBtn, &QPushButton::clicked, this, &Asn1Editor::setAsn1Value);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Asn1Editor::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

Asn1Editor::~Asn1Editor()
{
    delete ui;
}

void Asn1Editor::setAsn1Types(const QVariantList &asn1Types)
{
    if (m_asn1Types != asn1Types) {
        m_asn1Types = asn1Types;
        addAsn1TypeItems();
    }
}

const QVariantList &Asn1Editor::asn1Types() const
{
    return m_asn1Types;
}

void Asn1Editor::setFileName(const QString &fileName)
{
    if(m_fileName == fileName)
        return;
    m_fileName = fileName;
}

const QString &Asn1Editor::fileName() const
{
    return m_fileName;
}

void Asn1Editor::openFile()
{
    const QString filename = QFileDialog::getOpenFileName(
            this, tr("ASN1"), QString(), tr("ASN.1 files (*.asn);;XML files (*.xml);;All files (*.*)"));
    if (!filename.isEmpty())
        loadFile(filename);
}

void Asn1Editor::showParseError(const QString &error)
{
    QMessageBox::warning(this, tr("Error"), error);
}

void Asn1Editor::showAsn1Type(const QString &text)
{
    auto find = std::find_if(m_asn1Types.begin(), m_asn1Types.end(),
                             [&](const QVariant &value) { return value.toMap()["name"] == text; });

    if (find != m_asn1Types.end())
        m_asn1TreeView->setAsn1Model((*find).toMap());
}

void Asn1Editor::setAsn1Value()
{
    Asn1ValueParser valueParser;
    connect(&valueParser, &Asn1ValueParser::parseError, this, &Asn1Editor::showParseError);

    auto find = std::find_if(m_asn1Types.begin(), m_asn1Types.end(), [&](const QVariant &value) {
        return value.toMap()["name"] == ui->typesCB->currentText();
    });

    if (find != m_asn1Types.end())
        m_asn1TreeView->setAsn1Value(valueParser.parseAsn1Value((*find).toMap(), ui->valueEdit->toPlainText()));
}

void Asn1Editor::getAsn1Value()
{
    ui->valueEdit->setPlainText(m_asn1TreeView->getAsn1Value());
}

void Asn1Editor::loadFile(const QString &file)
{
    Asn1XMLParser parser;

    ui->typesCB->clear();
    connect(&parser, &Asn1XMLParser::parseError, this, &Asn1Editor::showParseError);

    QStringList errors;
    QFileInfo info(file);
    if (!info.suffix().compare("asn", Qt::CaseInsensitive)) {
        const QVariantList& types = parser.parseAsn1File(file, &errors);
        if (errors.isEmpty()) {
            m_asn1Types = types;
            m_fileName = info.fileName();
        } else {
            qWarning() << "Could not load file" << file;
            return;
        }
    } else {
        m_asn1Types = parser.parseAsn1XmlFile(file);
    }
    addAsn1TypeItems();
}

void Asn1Editor::addAsn1TypeItems()
{
    QStringList typeNames;

    ui->typesCB->clear();

    for (const auto &asn1Type : m_asn1Types)
        typeNames << asn1Type.toMap()["name"].toString();

    ui->typesCB->addItems(typeNames);
}

void Asn1Editor::accept()
{
    getAsn1Value();
    QDialog::accept();
}
} // namespace asn1
