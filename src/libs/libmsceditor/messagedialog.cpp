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

#include "aadlsystemchecks.h"
#include "asn1editor.h"
#include "asn1valueparser.h"
#include "chartlayoutmanager.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdsetasn1file.h"
#include "commands/cmdsetmessagedeclarations.h"
#include "commands/cmdsetparameterlist.h"
#include "file.h"
#include "iveditorcore.h"
#include "messagedeclarationsdialog.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "mscwriter.h"
#include "ui_messagedialog.h"

#include <QCompleter>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStyledItemDelegate>
#include <QTimer>

/*!
   \brief MessageDialog::MessageDialog

   The name is checked using a validator.
   But the parameters a only checked after editing using the msc parser, as the parameters can be
   quite complex. See paramaterDefn in the msc.g4 grammar file.
 */
MessageDialog::MessageDialog(msc::MscMessage *message, msc::ChartLayoutManager *charlayoutManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageDialog)
    , m_message(message)
    , m_chartLayoutManager(charlayoutManager)
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
    connect(ui->parameterTable, QOverload<QTableWidgetItem *>::of(&QTableWidget::itemDoubleClicked), this,
            &MessageDialog::editItem);
    connect(ui->parameterTable, &QTableWidget::cellChanged, this, &MessageDialog::checkTextValidity);

    connect(ui->declarationsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [&]() { ui->declarationButton->setEnabled(ui->declarationsComboBox->currentIndex() > 0); });
    connect(ui->declarationButton, &QPushButton::clicked, this, &MessageDialog::copyDeclarationData);
    connect(ui->editDeclarationsButton, &QPushButton::clicked, this, &MessageDialog::editDeclarations);

    connect(ui->nameLineEdit, &QLineEdit::textEdited, this, &MessageDialog::checkforEmptyCompleter);

    fillMessageDeclartionBox();
    selectDeclarationFromName();

    fillParameters();
    setParameterEditState();

    checkTextValidity();

    ui->nameLineEdit->setFocus();
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

/*!
   Sets the auto completion list for the name line edit
 */
void MessageDialog::setAadlConnectionNames(const QStringList &names)
{
    if (ui->nameLineEdit->completer()) {
        ui->nameLineEdit->completer()->deleteLater();
    }
    auto completer = new QCompleter(names, ui->nameLineEdit);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->nameLineEdit->setCompleter(completer);
    m_connectionNames = names;
}

/*!
   \brief MessageDialog::setAadlChecker
   \param checker
 */
void MessageDialog::setAadlChecker(msc::AadlSystemChecks *checker)
{
    m_aadlChecker = checker;
    const QString sourceName = m_message->sourceInstance() ? m_message->sourceInstance()->name() : "";
    const QString targetName = m_message->targetInstance() ? m_message->targetInstance()->name() : "";
    setAadlConnectionNames(m_aadlChecker->connectionNamesFromTo(sourceName, targetName));
}

void MessageDialog::accept()
{
    msc::MscCommandsStack *undoStack = m_chartLayoutManager->undoStack();
    undoStack->beginMacro("Edit message");
    undoStack->push(new msc::cmd::CmdEntityNameChange(m_message, ui->nameLineEdit->text(), m_chartLayoutManager));

    msc::MscParameterList parameters;
    for (int i = 0; i < ui->parameterTable->rowCount(); ++i) {
        const QString &text = ui->parameterTable->item(i, 0)->text();
        if (!text.isEmpty()) {
            parameters << msc::MscParameter(text);
        } else {
            qWarning() << "An empty parameter is not allowed";
        }
    }
    undoStack->push(new msc::cmd::CmdSetParameterList(m_message, parameters));
    undoStack->endMacro();

    QDialog::accept();
}

void MessageDialog::copyDeclarationData()
{
    const msc::MscMessageDeclarationList *declarations = messageDeclarations();
    if (!declarations)
        return;

    const int idx = ui->declarationsComboBox->currentIndex() - 1;
    if (idx < 0 || idx >= declarations->size())
        return;

    const msc::MscMessageDeclaration *declaration = declarations->at(idx);
    Q_ASSERT(declaration != nullptr);
    Q_ASSERT(!declaration->names().isEmpty());
    const QString name = declaration->names().at(0);
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
            break;
        }
    }
    if (ui->declarationsComboBox->currentIndex() != currentIdx)
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
    setParameterEditState();
    checkTextValidity();
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

    MessageDeclarationsDialog dialog(declarations, mscModel(), m_chartLayoutManager->undoStack(), m_aadlChecker, this);
    dialog.setFileName(model->dataDefinitionString());
    dialog.setAadlConnectionNames(m_connectionNames);

    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        m_chartLayoutManager->undoStack()->push(
                new msc::cmd::CmdSetMessageDeclarations(docs.at(0), dialog.declarations()));
        m_chartLayoutManager->undoStack()->push(new msc::cmd::CmdSetAsn1File(model, dialog.fileName(), "ASN.1"));
        fillMessageDeclartionBox();
        selectDeclarationFromName();
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
    setItemFlags(tableItem);
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

void MessageDialog::setParameterEditState()
{
    for (int row = 0; row < ui->parameterTable->rowCount(); ++row) {
        QTableWidgetItem *tableItem = ui->parameterTable->item(row, 0);
        if (!tableItem) {
            tableItem = new QTableWidgetItem();
            ui->parameterTable->setItem(row, 0, tableItem);
        }
        setItemFlags(tableItem);
    }
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

    const QSharedPointer<Asn1Acn::File> &types = mscModel()->asn1Types();
    asn1::Asn1Editor editor(types, this);
    const QString type = ui->parameterTable->verticalHeaderItem(item->row())->text();
    editor.showAsn1Type(type);
    editor.setValue(item->text());

    const int result = editor.exec();
    if (result == QDialog::Accepted)
        item->setText(editor.value());

    ui->parameterTable->closePersistentEditor(item);
}

void MessageDialog::setItemFlags(QTableWidgetItem *item)
{
    Qt::ItemFlags itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    if (m_selectedDeclaration)
        itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    item->setFlags(itemFlags);
}

void MessageDialog::checkTextValidity()
{
    msc::MscMessage msg;
    msg.setTargetInstance(m_message->targetInstance());
    msg.setSourceInstance(m_message->sourceInstance());

    msg.setName(ui->nameLineEdit->text());

    msc::MscParameterList parameters;
    for (int i = 0; i < ui->parameterTable->rowCount(); ++i) {
        QTableWidgetItem *item = ui->parameterTable->item(i, 0);
        if (item) {
            const QString &text = item->text();
            if (!text.isEmpty())
                parameters << msc::MscParameter(text);
            else
                m_isValid = false;
        }
    }
    msg.setParameters(parameters);

    msc::MscInstance *instance =
            m_message->sourceInstance() ? m_message->sourceInstance() : m_message->targetInstance();
    if (!instance)
        return;

    msc::MscWriter writer;
    QString text = writer.serialize(&msg, instance);
    text.remove("\n");

    try {
        msc::MscReader reader;
        const QString mscText = QString("msc chart; instance %1; %2 endinstance; endmsc;").arg(instance->name(), text);
        QScopedPointer<msc::MscModel> model(reader.parseText(mscText));
        m_isValid = true;
    } catch (...) {
        m_isValid = false;
    }

    if (m_selectedDeclaration) {
        Asn1Acn::Asn1ValueParser parser;
        const QSharedPointer<Asn1Acn::File> &asn1Data = mscModel()->asn1Types();
        if (!asn1Data.isNull()) {
            for (int i = 0; i < ui->parameterTable->rowCount(); ++i) {
                QTableWidgetItem *item = ui->parameterTable->item(i, 0);
                if (item) {
                    const QString &value = ui->parameterTable->item(i, 0)->text();
                    if (ui->parameterTable->verticalHeaderItem(i) != nullptr) {
                        const QString &typeName = ui->parameterTable->verticalHeaderItem(i)->text();
                        const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment = asn1Data->typeAssignment(typeName);
                        if (assignment) {
                            bool ok = true;
                            parser.parseAsn1Value(assignment->type(), value, &ok);
                            m_isValid = m_isValid && ok;
                        } else
                            m_isValid = false;
                    } else {
                        m_isValid = false;
                    }
                }
            }
        }
    }

    if (m_isValid)
        ui->previewLabel->setText("");
    else
        ui->previewLabel->setText(tr("<font color='red'>Invalid message:<br>%1</font>").arg(text));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_isValid);
}

/*!
   If the name is empty, the completer popup needs to be shown "manually"
   \note The popup is delayed, as otherwise it will be closed immediately after popup
 */
void MessageDialog::checkforEmptyCompleter()
{
    QTimer::singleShot(1, this, [this]() {
        if (ui->nameLineEdit->text().isEmpty() && ui->nameLineEdit->completer()) {
            ui->nameLineEdit->completer()->setCompletionPrefix("");
            ui->nameLineEdit->completer()->complete();
        }
    });
}

void MessageDialog::fillMessageDeclartionBox()
{
    ui->declarationsComboBox->clear();

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
        setItemFlags(tableItem);
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
