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

#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectiface.h"
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
#include "interface/commands/commandids.h"
#include "interface/commands/commandsfactory.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"
#include "ui_propertiesdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QListView>
#include <QPlainTextEdit>
#include <QTableView>
#include <QUndoCommand>

namespace aadlinterface {

PropertiesDialog::PropertiesDialog(
        aadl::AADLObject *obj, const QSharedPointer<Asn1Acn::File> &dataTypes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
    , m_cmdMacro(new cmd::CommandsStack::Macro(
              tr("Edit %1 - %2")
                      .arg(aadl::AADLNameValidator::nameOfType(m_dataObject->aadlType()).trimmed(),
                              m_dataObject->titleUI())))
    , m_dataTypes(dataTypes)
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
    case aadl::AADLObject::Type::FunctionType:
        return tr("Function Type");
    case aadl::AADLObject::Type::Function:
        return tr("Function");
    case aadl::AADLObject::Type::RequiredInterface:
        return tr("RI");
    case aadl::AADLObject::Type::ProvidedInterface:
        return tr("PI");
    case aadl::AADLObject::Type::Comment:
        return tr("Comment");
    case aadl::AADLObject::Type::Connection:
        return tr("Connection");
    case aadl::AADLObject::Type::ConnectionGroup:
        return tr("Connection Group");
    default:
        return QString();
    }
}

void PropertiesDialog::open()
{
    QDialog::open();
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
    case aadl::AADLObject::Type::FunctionType:
    case aadl::AADLObject::Type::Function: {
        initContextParams();
        initAttributesView();
        break;
    }
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface: {
        initIfaceParams();
        initAttributesView();
        break;
    }
    case aadl::AADLObject::Type::ConnectionGroup: {
        initConnectionGroup();
        initAttributesView();
        break;
    }
    case aadl::AADLObject::Type::Comment: {
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
    auto model = new AADLConnectionGroupModel(
            qobject_cast<aadl::AADLObjectConnectionGroup *>(m_dataObject), m_cmdMacro, this);
    auto connectionsView = new QListView;
    connectionsView->setModel(model);
    ui->tabWidget->insertTab(0, connectionsView, tr("Connections"));
}

void PropertiesDialog::initAttributesView()
{
    auto viewAttrs = new PropertiesViewBase(this);
    PropertiesListModel *modelAttrs { nullptr };

    switch (m_dataObject->aadlType()) {
    case aadl::AADLObject::Type::Function: {
        modelAttrs = new FunctionPropertiesListModel(m_cmdMacro, this);
        break;
    }
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface: {
        modelAttrs = new InterfacePropertiesListModel(m_cmdMacro, this);
        break;
    }
    default:
        modelAttrs = new InterfacePropertiesListModel(m_cmdMacro, this);
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
            ContextParametersModel::ColumnType, new PropertyTypeDelegate(m_dataTypes, viewAttrs->tableView()));
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
            IfaceParametersModel::ColumnType, new PropertyTypeDelegate(m_dataTypes, viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(IfaceParametersModel::ColumnEncoding,
            new StringListComboDelegate({ tr("NATIVE"), tr("UPER"), tr("ACN") }, // TODO: is it configurable?
                    viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(IfaceParametersModel::ColumnDirection,
            new StringListComboDelegate(
                    { aadl::IfaceParameter::directionName(aadl::IfaceParameter::Direction::In),
                            aadl::IfaceParameter::directionName(aadl::IfaceParameter::Direction::Out) },
                    viewAttrs->tableView()));
    viewAttrs->tableView()->horizontalHeader()->show();
    viewAttrs->setModel(modelIfaceParams);
    ui->tabWidget->insertTab(0, viewAttrs, tr("Parameters"));
}

void PropertiesDialog::initCommentView()
{
    if (auto comment = qobject_cast<aadl::AADLObjectComment *>(m_dataObject)) {
        auto commentEdit = new QPlainTextEdit(this);
        commentEdit->setPlainText(comment->titleUI());
        ui->tabWidget->insertTab(0, commentEdit, tr("Comment content"));
        connect(this, &QDialog::accepted, this, [comment, commentEdit]() {
            const QString text = commentEdit->toPlainText();
            if (comment->titleUI() == text)
                return;

            const QString encodedText = aadl::AADLNameValidator::encodeName(comment->aadlType(), text);
            const QVariantMap textArg { { aadl::meta::Props::token(aadl::meta::Props::Token::name), encodedText } };
            const QVariantList commentTextParams { QVariant::fromValue(comment), QVariant::fromValue(textArg) };
            auto commentTextCmd = cmd::CommandsFactory::create(cmd::ChangeEntityAttributes, commentTextParams);
            if (commentTextCmd) {
                commentTextCmd->setText(tr("Edit Comment"));
                cmd::CommandsStack::push(commentTextCmd);
            }
        });
    }
}

} // namespace aadlinterface
