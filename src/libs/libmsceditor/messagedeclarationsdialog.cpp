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

#include "asn1reader.h"
#include "asn1systemchecks.h"
#include "commands/cmdsetasn1file.h"
#include "definitions.h"
#include "msccommandsstack.h"
#include "mscdocument.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "mscwriter.h"
#include "systemchecks.h"
#include "typeassignment.h"
#include "ui_messagedeclarationsdialog.h"

#include <QCompleter>
#include <QDebug>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QRegExpValidator>
#include <QTimer>

MessageDeclarationsDialog::MessageDeclarationsDialog(msc::MscMessageDeclarationList *model, msc::MscModel *mscModel,
        msc::MscCommandsStack *undoStack, msc::SystemChecks *checker, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageDeclarationsDialog)
    , m_model(model->clone())
    , m_mscModel(mscModel)
    , m_undoStack(undoStack)
    , m_checker(checker)
{
    Q_ASSERT(m_model);
    Q_ASSERT(m_mscModel);
    ui->setupUi(this);
    QString regExPattern = msc::MscEntity::nameVerifier().pattern();
    regExPattern.insert(regExPattern.size() - 2, "|(\\s?,\\s?)"); // name as per spec + "," as separator
    QRegExp rx(regExPattern);
    QRegExpValidator *nameValidator = new QRegExpValidator(rx, this);
    ui->nameLineEdit->setValidator(nameValidator);

    ui->messagesView->setModel(m_model);

    updateAsn1TypesView();

    connect(ui->messagesView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &MessageDeclarationsDialog::showSelectedMessage);
    connect(ui->addDeclarationButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::addDeclaration);
    connect(ui->removeDeclarationButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::removeDeclaration);

    connect(ui->nameLineEdit, &QLineEdit::editingFinished, this, &MessageDeclarationsDialog::updateNames);

    connect(ui->usedListWidget, &QListWidget::itemSelectionChanged, this,
            &MessageDeclarationsDialog::updateParameterButtons);
    connect(ui->availableListView, &QListWidget::itemSelectionChanged, this,
            &MessageDeclarationsDialog::updateParameterButtons);

    connect(ui->addParameterButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::addSelectedParameter);
    connect(ui->removeParameterButton, &QPushButton::clicked, this,
            &MessageDeclarationsDialog::removeSelectedParameter);

    connect(ui->asn1Button, &QPushButton::clicked, this, &MessageDeclarationsDialog::selectAsn1File);

    connect(ui->nameLineEdit, &QLineEdit::textEdited, this, &MessageDeclarationsDialog::checkforEmptyCompleter);

    connect(m_mscModel, &msc::MscModel::asn1DataChanged, this, &MessageDeclarationsDialog::updateAsn1TypesView);

    ui->ivImportButton->setEnabled(m_checker && m_checker->hasValidSystem());
    connect(ui->ivImportButton, &QPushButton::clicked, this, &MessageDeclarationsDialog::importFromIV);

    updateDeclarationDetails();
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

void MessageDeclarationsDialog::setFileName(const QString &fileName)
{
    if (m_fileName == fileName)
        return;
    m_fileName = fileName;
}

const QString &MessageDeclarationsDialog::fileName() const
{
    return m_fileName;
}

/*!
   Sets the auto completion list for the name line edit
 */
void MessageDeclarationsDialog::setIVConnectionNames(const QStringList &names)
{
    if (ui->nameLineEdit->completer()) {
        ui->nameLineEdit->completer()->deleteLater();
    }
    auto completer = new QCompleter(names, ui->nameLineEdit);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->nameLineEdit->setCompleter(completer);
}

void MessageDeclarationsDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(event);
}

void MessageDeclarationsDialog::showSelectedMessage()
{
    if (m_selectedDeclaration)
        disconnect(m_selectedDeclaration, &msc::MscMessageDeclaration::dataChanged, this,
                &MessageDeclarationsDialog::updateDeclarationDetails);

    QItemSelectionModel *selectionModel = ui->messagesView->selectionModel();
    if (!selectionModel->hasSelection()) {
        ui->removeDeclarationButton->setEnabled(false);
        ui->nameLineEdit->setEnabled(false);
        m_selectedDeclaration = nullptr;
    } else {
        ui->nameLineEdit->setEnabled(true);
        ui->removeDeclarationButton->setEnabled(true);
        m_selectedDeclaration = m_model->at(selectionModel->selectedIndexes().at(0).row());
        connect(m_selectedDeclaration, &msc::MscMessageDeclaration::dataChanged, this,
                &MessageDeclarationsDialog::updateDeclarationDetails);
    }
    updateDeclarationDetails();
    updateParameterButtons();
}

void MessageDeclarationsDialog::addDeclaration()
{
    auto declaration = new msc::MscMessageDeclaration(m_model);
    declaration->setNames({ tr("message_name") });
    m_model->append(declaration);

    QModelIndex index = m_model->index(m_model->indexOf(declaration));
    ui->messagesView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    ui->nameLineEdit->setFocus();
    ui->nameLineEdit->selectAll();
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

void MessageDeclarationsDialog::addSelectedParameter()
{
    if (!m_selectedDeclaration)
        return;

    const QList<QListWidgetItem *> &items = ui->availableListView->selectedItems();
    if (items.isEmpty())
        return;

    QStringList types = m_selectedDeclaration->typeRefList();
    types.append(items.at(0)->text());
    m_selectedDeclaration->setTypeRefList(types);
}

void MessageDeclarationsDialog::removeSelectedParameter()
{
    if (!m_selectedDeclaration)
        return;

    const QModelIndexList &selection = ui->usedListWidget->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
        return;

    int idx = selection.at(0).row();

    QStringList types = m_selectedDeclaration->typeRefList();
    types.removeAt(idx);
    m_selectedDeclaration->setTypeRefList(types);
}

void MessageDeclarationsDialog::updateNames()
{
    if (!m_selectedDeclaration)
        return;

    if (ui->nameLineEdit->text().isEmpty())
        return;

    QStringList names = ui->nameLineEdit->text().split(",");
    for (QString &name : names) {
        name = name.trimmed();
    }

    m_selectedDeclaration->setNames(names);
}

void MessageDeclarationsDialog::updateDeclarationDetails()
{
    ui->usedListWidget->clear();

    if (!m_selectedDeclaration) {
        ui->nameLineEdit->clear();
        ui->resultLabel->setText("");
        ui->nameLineEdit->setEnabled(false);
        ui->usedListWidget->setEnabled(false);
        return;
    }

    ui->nameLineEdit->setEnabled(true);
    ui->usedListWidget->setEnabled(true);
    ui->nameLineEdit->setText(m_selectedDeclaration->names().join(", "));

    for (const QString &type : m_selectedDeclaration->typeRefList()) {
        ui->usedListWidget->addItem(type);
    }

    msc::MscWriter writer;
    ui->resultLabel->setText(writer.serialize(m_selectedDeclaration));
}

void MessageDeclarationsDialog::updateParameterButtons()
{
    bool selected = !ui->usedListWidget->selectedItems().isEmpty();
    ui->removeParameterButton->setEnabled(selected);

    selected = !ui->availableListView->selectedItems().isEmpty();
    ui->addParameterButton->setEnabled(selected && m_selectedDeclaration);
}

void MessageDeclarationsDialog::selectAsn1File()
{
    QString fileName = QFileDialog::getOpenFileName(this, "ASN.1 file", QString(), "ASN.1 files (*.asn)");
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QStringList errors;
        Asn1Acn::Asn1Reader parser;
        std::unique_ptr<Asn1Acn::File> types = parser.parseAsn1File(fileInfo, &errors);
        QSharedPointer<Asn1Acn::File> sharedTypes(types.release());
        if (errors.isEmpty()) {
            updateAsn1TypesView();
            m_fileName = fileInfo.fileName();
            m_undoStack->push(new msc::cmd::CmdSetAsn1File(m_mscModel, m_fileName, "ASN.1"));
        } else {
            qWarning() << "File" << fileName << "is no valid ASN.1 file:" << errors;
        }
    }
}

/*!
   If the name is empty, the completer popup needs to be shown "manually"
   \note The popup is delayed, as otherwise it will be closed immediately after popup
 */
void MessageDeclarationsDialog::checkforEmptyCompleter()
{
    QTimer::singleShot(1, this, [this]() {
        if (ui->nameLineEdit->text().isEmpty() && ui->nameLineEdit->completer()) {
            ui->nameLineEdit->completer()->setCompletionPrefix("");
            ui->nameLineEdit->completer()->complete();
        }
    });
}

/*!
   Adds all missing declarations from the iv model
 */
void MessageDeclarationsDialog::importFromIV()
{
    if (!m_checker || !m_checker->hasValidSystem()) {
        return;
    }

    QVector<msc::MscMessageDeclaration *> ivDeclarations = m_checker->allConnectionsAsDeclaration();

    while (!ivDeclarations.isEmpty()) {
        msc::MscMessageDeclaration *declaration = ivDeclarations.takeFirst();
        if (!m_model->exists(declaration)) {
            m_model->append(declaration);
        } else {
            delete declaration;
        }
    }
}

void MessageDeclarationsDialog::updateAsn1TypesView()
{
    ui->availableListView->clear();
    if (!m_mscModel || !m_mscModel->asn1Checks()) {
        return;
    }

    for (Asn1Acn::Definitions *definitions : m_mscModel->asn1Checks()->definitionsList()) {
        for (const QString &name : definitions->typeAssignmentNames()) {
            ui->availableListView->addItem(name);
        }
    }
}
