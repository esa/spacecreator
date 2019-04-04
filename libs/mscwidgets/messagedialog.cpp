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

#include "messagedialog.h"

#include "asn1editor.h"
#include "commands/common/commandsstack.h"
#include "messagedeclarationsdialog.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "mscwriter.h"
#include "ui_messagedialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QRegExp>
#include <QRegExpValidator>
#include <QUndoStack>

MessageDialog::MessageDialog(msc::MscMessage *message, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageDialog)
    , m_message(message)
{
    Q_ASSERT(message);
    ui->setupUi(this);

    QRegExpValidator *nameValidator = new QRegExpValidator(msc::MscEntity::nameVerifier(), this);
    ui->nameLineEdit->setValidator(nameValidator);

    ui->nameLineEdit->setText(m_message->name());

    connect(ui->nameLineEdit, &QLineEdit::editingFinished, this, &MessageDialog::selectDeclarationFromName);
    connect(ui->addParameterButton, &QPushButton::clicked, this, &MessageDialog::addParameter);
    connect(ui->removeParameterButton, &QPushButton::clicked, this, &MessageDialog::removeParameter);
    connect(ui->parameterTable->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &MessageDialog::checkRemoveButton);

    connect(ui->parameterTable, QOverload<QTableWidgetItem *>::of(&QTableWidget::itemActivated), this,
            &MessageDialog::editItem);

    connect(ui->declarationsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [&]() { ui->declarationButton->setEnabled(ui->declarationsComboBox->currentIndex() > 0); });
    connect(ui->declarationButton, &QPushButton::clicked, this, &MessageDialog::copyDeclarationData);
    connect(ui->editDeclarationsButton, &QPushButton::clicked, this, &MessageDialog::editDeclarations);

    fillMessageDeclartionBox();
    selectDeclarationFromName();

    fillParameters();
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::accept()
{
    msc::cmd::CommandsStack::current()->beginMacro("Edit message");
    msc::cmd::CommandsStack::push(
            msc::cmd::RenameEntity,
            { QVariant::fromValue(m_message.data()), QVariant::fromValue(ui->nameLineEdit->text()) });

    msc::MscParameterList parameters;
    for (int i = 0; i < ui->parameterTable->rowCount(); ++i) {
        const QString &text = ui->parameterTable->item(i, 0)->text();
        if (!text.isEmpty()) {
            parameters << msc::MscParameter(text);
        } else {
            qWarning() << "An empty parameter is not allowed";
        }
    }
    msc::cmd::CommandsStack::push(msc::cmd::SetParameterList,
                                  { QVariant::fromValue(m_message.data()), QVariant::fromValue(parameters) });
    msc::cmd::CommandsStack::current()->endMacro();

    QDialog::accept();
}

void MessageDialog::copyDeclarationData()
{
    const msc::MscMessageDeclarationList *declarations = messageDeclarations();
    if (!declarations)
        return;

    const int idx = ui->declarationsComboBox->currentIndex();
    if (idx == 0 || (idx - 1) >= declarations->size())
        return;

    const QString name = declarations->at(idx - 1)->names().at(0);
    ui->nameLineEdit->setText(name);
    selectDeclarationFromName();
}

void MessageDialog::selectDeclarationFromName()
{
    msc::MscMessageDeclarationList *declarations = messageDeclarations();
    if (!declarations)
        return;

    int currentIdx = 0;
    int i = 0;
    m_selectedDeclaration.clear();
    for (msc::MscMessageDeclaration *declaration : *declarations) {
        ++i;
        const QStringList &names = declaration->names();
        if (names.contains(ui->nameLineEdit->text())) {
            currentIdx = i;
            m_selectedDeclaration = declaration;
        }
    }
    ui->declarationsComboBox->setCurrentIndex(currentIdx);

    if (m_selectedDeclaration) {
        const QStringList &types = m_selectedDeclaration->typeRefList();
        ui->parameterTable->setRowCount(types.size());
        ui->parameterTable->setVerticalHeaderLabels(types);
        ui->parameterTable->verticalHeader()->setVisible(true);

        ui->addParameterButton->setEnabled(false);

    } else {
        ui->parameterTable->verticalHeader()->setVisible(false);
        ui->addParameterButton->setEnabled(true);
    }

    checkRemoveButton();
}

void MessageDialog::editDeclarations()
{
    msc::MscMessageDeclarationList *declarations = messageDeclarations();
    msc::MscModel *model = mscModel();
    if (!declarations || !model)
        return;

    const QVector<msc::MscDocument *> &docs = model->documents();
    if (docs.isEmpty())
        return;

    MessageDeclarationsDialog dialog(declarations, mscModel()->asn1TypesData(), this);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        const QVariantList cmdParams = { QVariant::fromValue<msc::MscDocument *>(docs.at(0)),
                                         QVariant::fromValue<msc::MscMessageDeclarationList *>(dialog.declarations()) };
        msc::cmd::CommandsStack::push(msc::cmd::Id::SetMessageDeclarations, cmdParams);
        model->setAsn1TypesData(dialog.asn1Types());
        model->setDataDefinitionString(dialog.fileName());
    }
}

void MessageDialog::addParameter()
{
    const int count = ui->parameterTable->rowCount();
    ui->parameterTable->setRowCount(count + 1);
    QTableWidgetItem *tableItem = ui->parameterTable->item(count, 0);
    if (!tableItem) {
        tableItem = new QTableWidgetItem();
        ui->parameterTable->setItem(count, 0, tableItem);
    }
    ui->parameterTable->editItem(tableItem);
}

void MessageDialog::removeParameter()
{
    QTableWidgetItem *tableItem = ui->parameterTable->currentItem();
    if (tableItem) {
        const int idx = tableItem->row();
        ui->parameterTable->removeRow(idx);
    }
}

void MessageDialog::checkRemoveButton()
{
    bool enabled = !m_selectedDeclaration && !ui->parameterTable->selectedItems().isEmpty();
    ui->removeParameterButton->setEnabled(enabled);
}

void MessageDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(event);
}

void MessageDialog::editItem(QTableWidgetItem *item)
{
    if (!m_selectedDeclaration)
        return;

    asn1::Asn1Editor editor(this);
    editor.setValueEditOnlyMode();
    const QVariantList &types = mscModel()->asn1TypesData();
    editor.setAsn1Types(types);
    const QString type = ui->parameterTable->verticalHeaderItem(item->row())->text();
    editor.showAsn1Type(type);
    editor.setValue(item->text());

    const int result = editor.exec();
    if (result == QDialog::Accepted)
        item->setText(editor.value());

    ui->parameterTable->closePersistentEditor(item);
}

void MessageDialog::fillMessageDeclartionBox()
{
    const msc::MscMessageDeclarationList *declarations = messageDeclarations();
    if (!declarations) {
        ui->declarationsComboBox->setEnabled(false);
        ui->declarationButton->setEnabled(false);
        return;
    }

    ui->declarationsComboBox->addItem(" ");

    for (const msc::MscMessageDeclaration *declaration : *declarations) {
        const QStringList &names = declaration->names();
        ui->declarationsComboBox->addItem(names.join(","));
    }
}

void MessageDialog::fillParameters()
{
    const int size = m_message->parameters().size();
    ui->parameterTable->setRowCount(size);
    int row = 0;
    for (const msc::MscParameter &parameter : m_message->parameters()) {
        QTableWidgetItem *tableItem = ui->parameterTable->item(row, 0);
        if (!tableItem) {
            tableItem = new QTableWidgetItem();
            ui->parameterTable->setItem(row, 0, tableItem);
        }
        tableItem->setText(parameter.parameter());
        ++row;
    }
}

msc::MscModel *MessageDialog::mscModel() const
{
    msc::MscModel *model = nullptr;
    const auto chart = qobject_cast<msc::MscChart *>(m_message->parent());
    if (chart) {
        msc::MscDocument *doc = chart->parentDocument();
        if (doc) {
            while (!model && doc) {
                model = qobject_cast<msc::MscModel *>(doc->parent());
                doc = doc->parentDocument();
            }
        } else {
            model = qobject_cast<msc::MscModel *>(chart->parent());
        }
    }

    Q_ASSERT(model);
    return model;
}

msc::MscMessageDeclarationList *MessageDialog::messageDeclarations() const
{
    const msc::MscModel *model = mscModel();
    if (model) {
        QVector<msc::MscDocument *> docs = model->documents();
        if (!docs.isEmpty())
            return docs.at(0)->messageDeclarations();
    }
    return nullptr;
}
