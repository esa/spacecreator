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
#include "file.h"
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
Asn1Editor::Asn1Editor(const QSharedPointer<Asn1Acn::File> &asn1Types, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Asn1Editor)
    , m_asn1Types(asn1Types)
{
    ui->setupUi(this);

    m_asn1TreeView = new Asn1TreeView(this);

    ui->verticalLayout->insertWidget(1, m_asn1TreeView);

    connect(ui->typesCB, &QComboBox::currentTextChanged, this, &Asn1Editor::showAsn1Type);
    connect(ui->valueBtn, &QPushButton::clicked, this, &Asn1Editor::setAsn1Value);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Asn1Editor::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setValueEditOnlyMode();
    addAsn1TypeItems();
}

Asn1Editor::~Asn1Editor()
{
    delete ui;
}

/*!
 * \brief Asn1Editor::setValue Set the current value
 * \param value
 */
void Asn1Editor::setValue(const QString &value)
{
    const QString &currentType { ui->typesCB->currentText() };
    if (value.isEmpty() || currentType.isEmpty() || !m_asn1Types) {
        return;
    }

    Asn1Acn::Asn1ValueParser valueParser;
    connect(&valueParser, &Asn1Acn::Asn1ValueParser::parseError, this, &Asn1Editor::showParseError);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item = m_asn1Types->typeAssignment(currentType);
    if (asn1Item) {
        m_asn1TreeView->setAsn1Value(valueParser.parseAsn1Value(asn1Item.get(), value));
    }
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
    ui->openBtn->setVisible(false);
    ui->typeLabel->setVisible(false);
    ui->typesCB->setVisible(false);
    ui->openBtn->setVisible(false);
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
    if (!m_asn1Types.isNull()) {
        const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item = m_asn1Types->typeAssignment(text);
        if (asn1Item) {
            m_asn1TreeView->setAsn1Model(asn1Item);
        }
    }

    if (ui->typesCB->currentText() != text) {
        ui->typesCB->setCurrentText(text);
    }
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

void Asn1Editor::addAsn1TypeItems()
{
    ui->typesCB->clear();
    if (!m_asn1Types) {
        return;
    }

    QStringList typeNames;
    for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : m_asn1Types->definitionsList()) {
        for (const std::unique_ptr<Asn1Acn::TypeAssignment> &type : definitions->types()) {
            typeNames << type->name();
        }
    }

    ui->typesCB->addItems(typeNames);
}

void Asn1Editor::accept()
{
    getAsn1Value();
    QDialog::accept();
}

}
