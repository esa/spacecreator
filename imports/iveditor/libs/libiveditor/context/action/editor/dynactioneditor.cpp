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

#include "dynactioneditor.h"

#include "actionsmodel.h"
#include "context/action/actionsmanager.h"
#include "conditiondialog.h"
#include "conditionsmodel.h"
#include "ui_dynactioneditor.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

namespace taste3 {
namespace ctx {

/*!
 * \class taste3::aadl::DynActionEditor
 * The UI for editing available scriptable actions.
 */

DynActionEditor::DynActionEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DynActionEditor)
    , m_actions()
    , m_actionsModel(new ActionsModel(this))
    , m_conditionsModel(new ConditionsModel(this))
{
    ui->setupUi(this);
    ui->lvActions->setModel(m_actionsModel);
    ui->lvConditions->setModel(m_conditionsModel);

    QStringList keyHolderes = ctx::ActionsManager::externalArgsHoldersDescr();
    ui->leAppCwd->setToolTip(keyHolderes.takeFirst());
    ui->leAppArgs->setToolTip(keyHolderes.join('\n'));

    connect(ui->lvActions->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &DynActionEditor::onActionActivated);

    connect(ui->lvConditions, &QListView::activated, this, &DynActionEditor::onConditionActivated);

    const QStringList actionTypes { tr("TASTE Action"), tr("External app") };
    ui->cbActType->addItems(actionTypes);

    ui->cbAction->addItems(ctx::ActionsManager::scriptableActionNames());

    on_cbActType_currentIndexChanged(0);
    displayAction(nullptr);

    setWindowTitle(tr("Context menu customization"));
}

DynActionEditor::~DynActionEditor()
{
    delete ui;
}

bool DynActionEditor::init()
{
    const QStringList &files = ctx::ActionsManager::listUserFiles();
    QString file;
    if (files.size() == 1)
        file = files.first();
    else {
        file = QFileDialog::getOpenFileName(this, tr("Select actions file"), ctx::ActionsManager::storagePath(),
                                            "*.json");
    }

    return loadFile(file);
}

bool DynActionEditor::loadFile(const QString &filePath)
{
    if (!QFile::exists(filePath))
        return false;

    m_actions.clear();
    m_action = nullptr;

    QString errMsg;
    m_actions = ctx::ActionsManager::parseFile(filePath, &errMsg);
    m_actionsModel->setupData(&m_actions);

    if (errMsg.isEmpty()) {
        ui->leFilePath->setText(filePath);
        return true;
    }

    return false;
}

void DynActionEditor::onActionActivated(const QModelIndex &current, const QModelIndex &)
{
    commitCurrentAction();

    if (current.isValid()) {
        const int row = current.row();
        if (row >= 0 && row < m_actions.size())
            m_action = &m_actions[row];
    }

    displayAction(m_action);
}

void DynActionEditor::displayAction(ctx::Action *action)
{
    ui->gbActionDetails->setEnabled(action);
    m_conditionsModel->setupData(action ? &action->m_conditions : nullptr);

    if (!action)
        return;

    const bool isExternal = action->m_internalActName.isEmpty();
    ui->cbActType->setCurrentIndex(isExternal ? 1 : 0);

    ui->cbAction->setCurrentText(!isExternal ? action->m_internalActName : QString());
    ui->leApp->setText(isExternal ? action->m_externalApp : QString());
    ui->leAppArgs->setText(isExternal ? action->m_externalAppParams.join(" ") : QString());
    ui->leAppCwd->setText(isExternal ? action->m_externalAppCwd : QString());
}

void DynActionEditor::on_cbActType_currentIndexChanged(int id)
{
    const bool isExternal = id != 0;
    ui->labelAction->setVisible(!isExternal);
    ui->labelApp->setVisible(isExternal);
    ui->labelAppArgs->setVisible(isExternal);
    ui->labelAppCwd->setVisible(isExternal);

    ui->cbAction->setVisible(!isExternal);
    ui->leApp->setVisible(isExternal);
    ui->leAppArgs->setVisible(isExternal);
    ui->leAppCwd->setVisible(isExternal);
}

void DynActionEditor::commitCurrentAction()
{
    if (!m_action)
        return;

    const bool isExternal = ui->cbActType->currentIndex() != 0;
    m_action->m_internalActName = !isExternal ? ui->cbAction->currentText() : QString();
    m_action->m_externalApp = isExternal ? ui->leApp->text() : QString();
    m_action->m_externalAppParams = isExternal ? ui->leAppArgs->text().split(" ") : QStringList();
    m_action->m_externalAppCwd = isExternal ? ui->leAppCwd->text() : QString();

    m_action = nullptr;
}

void DynActionEditor::on_btnCreateAction_clicked()
{
    QString newName = QInputDialog::getText(nullptr, tr("New action"), tr("Title:"));
    if (!newName.isEmpty()) {
        auto readNames = [](const QVector<ctx::Action> &actions) {
            QStringList res;
            for (const ctx::Action &action : actions)
                res.append(action.m_title);
            return res;
        };
        const QStringList names = readNames(m_actions);
        QString name(newName);
        int counter(0);
        while (names.contains(name))
            name = newName + "_#" + QString::number(++counter);

        commitCurrentAction();

        const QModelIndex &id = m_actionsModel->createAction(name, ui->cbAction->itemText(0));
        if (id.isValid()) {
            ui->lvActions->setCurrentIndex(id);
        }
    }
}

void DynActionEditor::on_btnRemoveAction_clicked()
{
    const QModelIndex &id = ui->lvActions->selectionModel()->currentIndex();
    if (!id.isValid())
        return;

    m_actionsModel->removeRow(id.row());
}

void DynActionEditor::on_btnCreateCondition_clicked()
{
    m_conditionsModel->insertRows(m_conditionsModel->rowCount(), 1);
}

void DynActionEditor::on_btnRemoveCondition_clicked()
{
    const QModelIndex &currId = ui->lvConditions->selectionModel()->currentIndex();
    if (!currId.isValid())
        return;
    m_conditionsModel->removeRows(currId.row(), 1);
}

void DynActionEditor::onConditionActivated(const QModelIndex &index)
{
    if (!index.isValid() || !m_action)
        return;

    const ctx::Condition cond = m_action->m_conditions.at(index.row());
    const QStringList knowItems = QStringList({ "*" }) + ctx::Condition::knownTypes();
    ConditionDialog *dlg = new ConditionDialog(cond, knowItems, this);
    if (dlg->exec() == QDialog::Accepted) {
        m_action->m_conditions.replace(index.row(), dlg->condition());
    }
    delete dlg;
}

void DynActionEditor::accept()
{
    if (save()) {
        QDialog::accept();
        ctx::ActionsManager::reload();
    }
}

bool DynActionEditor::save()
{
    commitCurrentAction();

    QJsonArray jArr;
    for (const ctx::Action &act : m_actions)
        jArr.append(act.toJson());

    const QString &filePath = ui->leFilePath->text();
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "Can't open file" << filePath << "for writing:" << f.errorString();
        return false;
    }
    f.write(QJsonDocument(jArr).toJson());

    return true;
}

void DynActionEditor::on_btnSelectFile_clicked()
{
    const QString filePath =
            QFileDialog::getOpenFileName(this, tr("Select file"), ctx::ActionsManager::storagePath(), "*.json");
    if (!filePath.isEmpty())
        loadFile(filePath);
}

void DynActionEditor::on_btnCreateFile_clicked()
{
    const QString &filePath =
            QFileDialog::getSaveFileName(this, tr("Save new file"), ctx::ActionsManager::storagePath(), "*.json");
    if (!filePath.isEmpty()) {
        m_actions.clear();
        m_action = nullptr;

        m_actionsModel->setupData(&m_actions);

        ui->leFilePath->setText(filePath);
    }
}

}
}
