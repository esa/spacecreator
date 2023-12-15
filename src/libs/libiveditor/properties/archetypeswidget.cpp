/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "archetypeswidget.h"

#include "archetypes/archetypemodel.h"
#include "archetypes/archetypeobject.h"
#include "archetypes/functionarchetype.h"
#include "archetypes/interfacearchetype.h"
#include "archetypes/parameterarchetype.h"
#include "archetypeswidgetmodel.h"
#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "asn1/types/boolean.h"
#include "commands/cmdfunctionarchetypesapply.h"
#include "commands/cmdinterfaceitemcreate.h"
#include "delegates/comboboxdelegate.h"
#include "ivarchetypereference.h"
#include "ivconnectionlayertype.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "shared/veobject.h"
#include "ui_archetypeswidget.h"

#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

namespace ive {

ArchetypesWidget::ArchetypesWidget(ivm::ArchetypeModel *archetypeModel, ivm::IVModel *layersModel,
        QPointer<Asn1Acn::Asn1SystemChecks> asn1Checks, ivm::IVFunctionType *function, shared::cmd::CommandsStackBase::Macro *macro,
        QWidget *parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::ArchetypesWidget>())
    , m_archetypeModel(archetypeModel)
    , m_layersModel(layersModel)
    , m_asn1Checks(asn1Checks)
    , m_cmdMacro(macro)
{
    Q_ASSERT(function && function->model());
    m_ui->setupUi(this);

    m_ui->tableView->setItemDelegateForColumn(ArchetypesWidgetModel::Column::LibraryName,
            new ive::ComboBoxDelegate(archetypeModel->getLibrariesNames(), m_ui->tableView));
    m_ui->tableView->setItemDelegateForColumn(
            ArchetypesWidgetModel::Column::FunctionName, new ive::ComboBoxDelegate(QStringList(), m_ui->tableView));

    m_model = new ArchetypesWidgetModel(archetypeModel, macro, this);
    m_model->setFunction(function);
    m_ui->tableView->setModel(m_model);
    m_ui->tableView->horizontalHeader()->resizeSection(0, 220);
    m_ui->tableView->horizontalHeader()->resizeSection(1, 180);

    connect(m_ui->addButton, &QPushButton::clicked, this, &ArchetypesWidget::addArchetype);
    connect(m_ui->deleteButton, &QPushButton::clicked, this, &ArchetypesWidget::deleteArchetype);

    connect(m_model, &ArchetypesWidgetModel::rowsInserted, this, &ArchetypesWidget::rowsInserted);
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);
}

ArchetypesWidget::~ArchetypesWidget() {}

void ArchetypesWidget::addArchetype()
{
    int newRow = m_model->rowCount();

    if (!m_model->insertRow(newRow)) {
        return;
    }

    QModelIndex idx = m_model->index(newRow, ArchetypesWidgetModel::Column::LibraryName);
    m_ui->tableView->edit(idx);
    m_ui->tableView->scrollToBottom();
    m_ui->tableView->selectRow(newRow);
}

void ArchetypesWidget::deleteArchetype()
{
    QModelIndexList selections = m_ui->tableView->selectionModel()->selectedRows();
    if (selections.size() != 1) {
        return;
    }

    m_model->removeRow(selections.at(0).row());
}

void ArchetypesWidget::applyArchetypes()
{
    if (!checkReferences() || !m_model->areArchetypesModified()) {
        return;
    }

    if (!handleMissingTypeNames()) {
        return;
    }

    auto command = new cmd::CmdFunctionArchetypesApply(m_model->getFunction(), m_model->getArchetypeReferences());
    m_cmdMacro->push(command);

    for (int i = 0; i < m_model->getArchetypeReferences().size(); i++) {
        ivm::ArchetypeObject *archetypeObject = m_archetypeModel->getObjectByName(
                m_model->getArchetypeReferences()[i]->getFunctionName(), ivm::ArchetypeObject::Type::FunctionArchetype);
        ivm::FunctionArchetype *functionArchetype = archetypeObject->as<ivm::FunctionArchetype *>();

        if (functionArchetype == nullptr || !m_model->isReferenceNew(i)) {
            continue;
        }

        for (auto interface : functionArchetype->getInterfaces())
        {
            const auto ivFunction = m_model->getFunction();
            if (ivFunction->hasInterface(interface->title()))
            {
                const auto existedInterface = ivFunction->getInterfaceByName(interface->title());
                const auto existedInterfaceType = existedInterface->direction();
                const auto interfaceType = interface->getInterfaceType();

                if (((existedInterfaceType == ivm::IVInterface::InterfaceType::Required) && (interfaceType == ivm::InterfaceArchetype::InterfaceType::REQUIRED)) ||
                    ((existedInterfaceType == ivm::IVInterface::InterfaceType::Provided) && (interfaceType == ivm::InterfaceArchetype::InterfaceType::PROVIDED)))
                {
                    const auto directionName = interfaceType == ivm::InterfaceArchetype::InterfaceType::REQUIRED ? "required" : "provided";
                    QMessageBox::warning(qApp->activeWindow(), tr("Duplicate interface"), 
                                        tr("Function \"%1\" already contains the interface called \"%2\" with \"%3\" direction")
                                        .arg(ivFunction->title())
                                        .arg(interface->title())
                                        .arg(directionName));
                    continue;
                }
            }

            ivm::IVInterface::CreationInfo creationInfo = generateCreationInfo(interface);

            auto command = new cmd::CmdInterfaceItemCreate(creationInfo);
            m_cmdMacro->push(command);

            m_model->setReferenceNotNew(i);
        }
    }
}

bool ArchetypesWidget::checkReferences()
{
    QVector<ivm::IVArchetypeReference *> references = m_model->getArchetypeReferences();

    for (auto reference : references) {
        if (reference->getLibraryName().isEmpty() || reference->getFunctionName().isEmpty()) {
            QMessageBox::warning(
                    qApp->activeWindow(), tr("Archetype error"), tr("Archetypes implementations are incomplete"));
            return false;
        }
    }

    if (shared::VEObject::hasDuplicates(references)) {
        QMessageBox::warning(
                qApp->activeWindow(), tr("Archetype error"), tr("Archetype list has duplicate implementations"));
        return false;
    }

    return true;
}

ivm::IVInterface::CreationInfo ArchetypesWidget::generateCreationInfo(ivm::InterfaceArchetype *interfaceArchetype)
{
    ivm::IVInterface::CreationInfo creationInfo;

    switch (interfaceArchetype->getInterfaceType()) {
    case ivm::InterfaceArchetype::InterfaceType::PROVIDED:
        creationInfo.type = ivm::IVInterface::InterfaceType::Provided;
        break;
    case ivm::InterfaceArchetype::InterfaceType::REQUIRED:
        creationInfo.type = ivm::IVInterface::InterfaceType::Required;
        break;
    }

    switch (interfaceArchetype->getOperationKind()) {
    case ivm::InterfaceArchetype::OperationKind::CYCLIC:
        creationInfo.kind = ivm::IVInterface::OperationKind::Cyclic;
        break;
    case ivm::InterfaceArchetype::OperationKind::SPORADIC:
        creationInfo.kind = ivm::IVInterface::OperationKind::Sporadic;
        break;
    case ivm::InterfaceArchetype::OperationKind::PROTECTED:
        creationInfo.kind = ivm::IVInterface::OperationKind::Protected;
        break;
    case ivm::InterfaceArchetype::OperationKind::UNPROTECTED:
        creationInfo.kind = ivm::IVInterface::OperationKind::Unprotected;
        break;
    }

    ivm::IVObject *object =
            m_layersModel->getObjectByName(interfaceArchetype->getLayer(), ivm::IVObject::Type::ConnectionLayer);
    ivm::IVConnectionLayerType *layer = object->as<ivm::IVConnectionLayerType *>();

    if (layer == nullptr) {
        layer = new ivm::IVConnectionLayerType();
        layer->rename(interfaceArchetype->getLayer());
        m_layersModel->addObject(layer);
    }

    creationInfo.parameters = generateInterfaceParameters(interfaceArchetype->getParameters());
    creationInfo.layer = layer;
    creationInfo.function = m_model->getFunction();
    creationInfo.model = m_model->getFunction()->model();
    creationInfo.name = interfaceArchetype->title();

    return creationInfo;
}

QVector<shared::InterfaceParameter> ArchetypesWidget::generateInterfaceParameters(
        QVector<ivm::ParameterArchetype *> parameters)
{
    QVector<shared::InterfaceParameter> resultParameters;

    for (auto archetypeParameter : parameters) {
        shared::InterfaceParameter::Direction parameterDirection;
        switch (archetypeParameter->getDirection()) {
        case ivm::ParameterArchetype::ParameterDirection::IN:
            parameterDirection = shared::InterfaceParameter::Direction::IN;
            break;
        case ivm::ParameterArchetype::ParameterDirection::OUT:
            parameterDirection = shared::InterfaceParameter::Direction::OUT;
            break;
        }

        shared::InterfaceParameter parameter(archetypeParameter->title(), shared::BasicParameter::Type::Other,
                archetypeParameter->getType(), QObject::tr("NATIVE"), parameterDirection);

        resultParameters.append(parameter);
    }

    return resultParameters;
}

void ArchetypesWidget::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int i = first; i <= last; ++i) {
        const QModelIndex functionIndex = m_model->index(i, ArchetypesWidgetModel::Column::FunctionName, parent);
        if (functionIndex.isValid()) {
            m_ui->tableView->openPersistentEditor(functionIndex);
        }
    }
}

bool ArchetypesWidget::handleMissingTypeNames()
{
    const auto missingTypeNames = findMissingTypeNames();
    if (missingTypeNames.isEmpty()) {
        return true;
    }

    const auto missingTypesString = missingTypeNames.join(QChar('\n'));
    const auto reply = QMessageBox::question(qApp->activeWindow(), tr("Missing types"),
            tr("Archetypes contain missing type names:\n\n%1\n\nWould you like add type stubs to the project and continue?").arg(missingTypesString),
            QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        return false;
    }

    Asn1Acn::Asn1ModelStorage* asn1Storage = m_asn1Checks->asn1Storage();
    Asn1Acn::File* asn1DataTypes = asn1Storage-> asn1DataTypes(m_asn1Checks->primaryFileName());

    QString definitionsString = buildDefinitionsString(missingTypeNames);
    bool success = writeDefinitions(m_asn1Checks->primaryFileName(), definitionsString);
    if (!success) {
        QMessageBox::critical(qApp->activeWindow(), tr("Write error"), tr("Unable to write types into the ASN.1 file."));
        return false;
    }
    return true;
}

QStringList ArchetypesWidget::findMissingTypeNames()
{
     const auto allTypeNames = m_asn1Checks->allTypeNames();
     QStringList missingTypeNames;

     // check for missing types
     for (int i = 0; i < m_model->getArchetypeReferences().size(); i++) {
         ivm::ArchetypeObject *archetypeObject = m_archetypeModel->getObjectByName(
                 m_model->getArchetypeReferences()[i]->getFunctionName(), ivm::ArchetypeObject::Type::FunctionArchetype);
         ivm::FunctionArchetype *functionArchetype = archetypeObject->as<ivm::FunctionArchetype *>();

         if (functionArchetype == nullptr || !m_model->isReferenceNew(i)) {
             continue;
         }

         for (auto interface : functionArchetype->getInterfaces()) {
             // compare with ASN for missing types
             const auto parameters = interface->getParameters();
             for (auto parameter : parameters) {
                 const auto type = parameter-> getType();
                 if (!allTypeNames.contains(type)) {
                     missingTypeNames.append(type);
                 }
             }
         }
     }
     missingTypeNames.removeDuplicates();
     return missingTypeNames;
}

QString ArchetypesWidget::buildDefinitionsString(const QStringList& missingTypeNames)
{
    QString definitionsString;
    for (const auto missingTypeName : missingTypeNames) {
        definitionsString.append(QStringLiteral("  %1 ::= BOOLEAN\n").arg (missingTypeName));
    }
    definitionsString.append(QStringLiteral("\nEND\n"));
    return definitionsString;
}

bool ArchetypesWidget::writeDefinitions(const QString &fileName, const QString& definitionsString)
{
    QFile asnFile(m_asn1Checks->primaryFileName());
    if (!asnFile.open(QFile::ReadWrite)) {
        return false;
    }

    const auto asnFileContent = QString(asnFile.readAll());
    QString regexString;
    // END immediately follows newline
    regexString.append("(?:\\n(END))");
    // alternative
    regexString.append("|");
    // newline and a group of any characters, but without a comment token (--)
    regexString.append("(?:\\n(?:(?!--).)*");
    // at least one white space character excluding newlines,
    // to ensure that END is not a substring in another keyword
    regexString.append("[^\\S\\r\\n]+");
    // END keyword
    regexString.append("(END))");
    QRegularExpression regex(regexString);

    // QRegularExpressionMatchIterator can only iterate forward - looping over until the last match
    auto matches = regex.globalMatch(asnFileContent);
    QRegularExpressionMatch match;
    while (matches.hasNext()) {
        match = matches.next();
    }

    if (!match.isValid()) {
        return true;
    }

    // rebuild and rewrite file content
    const auto lastCapturedIndex = match.lastCapturedIndex();
    QString newAsnFileContent = QStringLiteral("%1%2%3").arg(
            asnFileContent.mid(0, match.capturedStart(lastCapturedIndex)), definitionsString,
            asnFileContent.mid(match.capturedEnd(lastCapturedIndex)));
    asnFile.resize(0);
    asnFile.write(newAsnFileContent.toUtf8());
    return true;
}

} // namespace ive
