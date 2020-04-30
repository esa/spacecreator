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

/*!
 * \class Asn1Editor
 *
 * This is the main dialog that the user sees to manipulate the ASN.1 contents.
 */

/*!
 * \brief Asn1Editor::Asn1Editor Constructor
 * \param parent Dialog parent
 */
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

/*!
 * \brief Asn1Editor::setAsn1Types Set up the list of ASN.1 types
 * \param asn1Types
 */
void Asn1Editor::setAsn1Types(const QVariantList &asn1Types)
{
    if (m_asn1Types != asn1Types) {
        m_asn1Types = asn1Types;
        addAsn1TypeItems();
    }
}

/*!
 * \brief Asn1Editor::asn1Types Get the list of ASN.1 types
 * \return
 */
const QVariantList &Asn1Editor::asn1Types() const
{
    return m_asn1Types;
}

/*!
 * \brief Asn1Editor::setFileName Set the current filename
 * \param fileName
 */
void Asn1Editor::setFileName(const QString &fileName)
{
    if (m_fileName == fileName)
        return;
    m_fileName = fileName;
}

const QString &Asn1Editor::fileName() const
{
    return m_fileName;
}

/*!
 * \brief Asn1Editor::setValue Set the current value
 * \param value
 */
void Asn1Editor::setValue(const QString &value)
{
    const QString &currentType { ui->typesCB->currentText() };
    if (value.isEmpty() || currentType.isEmpty())
        return;

    Asn1ValueParser valueParser;
    connect(&valueParser, &Asn1ValueParser::parseError, this, &Asn1Editor::showParseError);

    auto find = std::find_if(m_asn1Types.begin(), m_asn1Types.end(),
                             [&](const QVariant &value) { return value.toMap()["name"] == currentType; });

    if (find != m_asn1Types.end())
        m_asn1TreeView->setAsn1Value(valueParser.parseAsn1Value((*find).toMap(), value));
}

/*!
 * \brief Asn1Editor::value Get the current value as a string
 * \return
 */
QString Asn1Editor::value() const
{
    return m_asn1TreeView->getAsn1Value();
}

/*!
 * \brief Asn1Editor::setValueEditOnlyMode After this is called, the user can only modify the value
 */
void Asn1Editor::setValueEditOnlyMode()
{
    ui->typeLabel->setVisible(false);
    ui->typesCB->setVisible(false);
    ui->openBtn->setVisible(false);
}

/*!
 * \brief Asn1Editor::openFile Show an open dialog and open an ASN.1 file.
 */
void Asn1Editor::openFile()
{
    const QString filename = QFileDialog::getOpenFileName(
            this, tr("ASN1"), QString(), tr("ASN.1 files (*.asn);;XML files (*.xml);;All files (*.*)"));
    if (!filename.isEmpty())
        loadFile(filename);
}

/*!
 * \brief Asn1Editor::showParseError Show a parser error in a message box
 * \param error
 */
void Asn1Editor::showParseError(const QString &error)
{
    QMessageBox::warning(this, tr("Error"), error);
}

/*!
 * \brief Asn1Editor::showAsn1Type Show the value of the \a text parameter
 */
void Asn1Editor::showAsn1Type(const QString &text)
{
    auto find = std::find_if(m_asn1Types.begin(), m_asn1Types.end(),
                             [&](const QVariant &value) { return value.toMap()["name"] == text; });

    if (find != m_asn1Types.end())
        m_asn1TreeView->setAsn1Model((*find).toMap());

    if (ui->typesCB->currentText() != text)
        ui->typesCB->setCurrentText(text);
}

/*!
 * \brief Asn1Editor::setAsn1Value Set the ASN.1 value to the contents of the string
 */
void Asn1Editor::setAsn1Value()
{
    setValue(ui->valueEdit->toPlainText());
}

/*!
 * \brief Asn1Editor::getAsn1Value Update the string in the view to the current value
 */
void Asn1Editor::getAsn1Value()
{
    ui->valueEdit->setPlainText(m_asn1TreeView->getAsn1Value());
}

/*!
 * \brief Asn1Editor::loadFile Load an ASN.1 file
 * \param file
 */
void Asn1Editor::loadFile(const QString &file)
{
    Asn1XMLParser parser;

    ui->typesCB->clear();
    connect(&parser, &Asn1XMLParser::parseError, this, &Asn1Editor::showParseError);

    QStringList errors;
    QFileInfo info(file);
    if (!info.suffix().compare("asn", Qt::CaseInsensitive)) {
        const QVariantList &types = parser.parseAsn1File(file, &errors);
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
