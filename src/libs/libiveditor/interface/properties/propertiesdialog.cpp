/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertiesdialog.h"

#include "aadlcomment.h"
#include "aadlconnectiongroup.h"
#include "aadliface.h"
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "commandsstack.h"
#include "contextparametersmodel.h"
#include "delegates/asn1valuedelegate.h"
#include "delegates/attributedelegate.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/functionattrdelegate.h"
#include "delegates/interfaceattrdelegate.h"
#include "delegates/propertytypedelegate.h"
#include "ifaceparametersmodel.h"
#include "interface/aadlconnectiongroupmodel.h"
#include "interface/commands/cmdentityattributechange.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"
#include "ui_propertiesdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QListView>
#include <QPlainTextEdit>
#include <QTableView>
#include <QUndoCommand>

namespace ive {

PropertiesDialog::PropertiesDialog(ivm::PropertyTemplateConfig *dynPropConfig, ivm::AADLObject *obj,
        const QSharedPointer<Asn1Acn::File> &dataTypes, cmd::CommandsStack *commandsStack, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
    , m_dynPropConfig(dynPropConfig)
    , m_cmdMacro(new cmd::CommandsStack::Macro(commandsStack->undoStack(),
              tr("Edit %1 - %2")
                      .arg(ivm::AADLNameValidator::nameOfType(m_dataObject->aadlType()).trimmed(),
                              m_dataObject->titleUI())))
    , m_dataTypes(dataTypes)
    , m_commandsStack(commandsStack)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    initTabs();
}

PropertiesDialog::~PropertiesDialog()
{
    delete m_cmdMacro;
    delete ui;
}

QString PropertiesDialog::objectTypeName() const
{
    if (!m_dataObject)
        return QString();

    switch (m_dataObject->aadlType()) {
    case ivm::AADLObject::Type::FunctionType:
        return tr("Function Type");
    case ivm::AADLObject::Type::Function:
        return tr("Function");
    case ivm::AADLObject::Type::RequiredInterface:
        return tr("RI");
    case ivm::AADLObject::Type::ProvidedInterface:
        return tr("PI");
    case ivm::AADLObject::Type::Comment:
        return tr("Comment");
    case ivm::AADLObject::Type::Connection:
        return tr("Connection");
    case ivm::AADLObject::Type::ConnectionGroup:
        return tr("Connection Group");
    default:
        return QString();
    }
}

void PropertiesDialog::done(int r)
{
    m_cmdMacro->setComplete(r == QDialog::Accepted);
    QDialog::done(r);
}

void PropertiesDialog::initTabs()
{
    if (!m_dataObject)
        return;

    switch (m_dataObject->aadlType()) {
    case ivm::AADLObject::Type::FunctionType:
    case ivm::AADLObject::Type::Function: {
        initContextParams();
        initAttributesView();
        break;
    }
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface: {
        initIfaceParams();
        initAttributesView();
        break;
    }
    case ivm::AADLObject::Type::ConnectionGroup: {
        initConnectionGroup();
        initAttributesView();
        break;
    }
    case ivm::AADLObject::Type::Comment: {
        initCommentView();
        break;
    }
    default:
        break;
    }

    setWindowTitle(tr("Edit %1").arg(objectTypeName()));
    ui->tabWidget->setCurrentIndex(0);
}

void PropertiesDialog::initConnectionGroup()
{
    auto model = new AADLConnectionGroupModel(qobject_cast<ivm::AADLConnectionGroup *>(m_dataObject), m_cmdMacro, this);
    auto connectionsView = new QListView;
    connectionsView->setModel(model);
    ui->tabWidget->insertTab(0, connectionsView, tr("Connections"));
}

void PropertiesDialog::initAttributesView()
{
    auto viewAttrs = new PropertiesViewBase(this);
    PropertiesListModel *modelAttrs { nullptr };

    switch (m_dataObject->aadlType()) {
    case ivm::AADLObject::Type::Function: {
        modelAttrs = new FunctionPropertiesListModel(m_cmdMacro, m_dynPropConfig, this);
        break;
    }
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface: {
        modelAttrs = new InterfacePropertiesListModel(m_cmdMacro, m_dynPropConfig, this);
        break;
    }
    default:
        modelAttrs = new InterfacePropertiesListModel(m_cmdMacro, m_dynPropConfig, this);
        break;
    }

    modelAttrs->setDataObject(m_dataObject);
    viewAttrs->setModel(modelAttrs);
    viewAttrs->tableView()->setItemDelegateForColumn(
            PropertiesListModel::ColumnValue, new AttributeDelegate(viewAttrs->tableView()));

    ui->tabWidget->insertTab(0, viewAttrs, tr("Attributes"));
}

void PropertiesDialog::initContextParams()
{
    ContextParametersModel *modelCtxParams = new ContextParametersModel(m_cmdMacro, this);
    modelCtxParams->setDataTypes(m_dataTypes);
    modelCtxParams->setDataObject(m_dataObject);

    PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
    viewAttrs->tableView()->setItemDelegateForColumn(
            ContextParametersModel::ColumnType, new ContextParametersTypeDelegate(m_dataTypes, viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(
            ContextParametersModel::ColumnValue, new Asn1ValueDelegate(m_dataTypes, viewAttrs->tableView()));
    viewAttrs->tableView()->horizontalHeader()->show();
    viewAttrs->setModel(modelCtxParams);
    ui->tabWidget->insertTab(0, viewAttrs, tr("Context Parameters"));
}

void PropertiesDialog::initIfaceParams()
{
    IfaceParametersModel *modelIfaceParams = new IfaceParametersModel(m_cmdMacro, this);
    modelIfaceParams->setDataObject(m_dataObject);

    PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
    viewAttrs->tableView()->setItemDelegateForColumn(
            IfaceParametersModel::ColumnType, new IfaceParametersTypeDelegate(m_dataTypes, viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(IfaceParametersModel::ColumnEncoding,
            new StringListComboDelegate({ tr("NATIVE"), tr("UPER"), tr("ACN") }, // TODO: is it configurable?
                    viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(IfaceParametersModel::ColumnDirection,
            new StringListComboDelegate(
                    { ivm::IfaceParameter::directionName(ivm::IfaceParameter::Direction::In),
                            ivm::IfaceParameter::directionName(ivm::IfaceParameter::Direction::Out) },
                    viewAttrs->tableView()));
    viewAttrs->tableView()->horizontalHeader()->show();
    viewAttrs->setModel(modelIfaceParams);
    ui->tabWidget->insertTab(0, viewAttrs, tr("Parameters"));
}

void PropertiesDialog::initCommentView()
{
    if (auto comment = qobject_cast<ivm::AADLComment *>(m_dataObject)) {
        auto commentEdit = new QPlainTextEdit(this);
        commentEdit->setPlainText(comment->titleUI());
        ui->tabWidget->insertTab(0, commentEdit, tr("Comment content"));
        connect(this, &QDialog::accepted, this, [comment, commentEdit, this]() {
            const QString text = commentEdit->toPlainText();
            if (comment->titleUI() == text)
                return;

            const QString encodedText = ivm::AADLNameValidator::encodeName(comment->aadlType(), text);
            const QVariantHash textArg { { ivm::meta::Props::token(ivm::meta::Props::Token::name), encodedText } };
            auto commentTextCmd = new cmd::CmdEntityAttributeChange(comment, textArg);
            commentTextCmd->setText(tr("Edit Comment"));
            m_commandsStack->push(commentTextCmd);
        });
    }
}

} // namespace ive
