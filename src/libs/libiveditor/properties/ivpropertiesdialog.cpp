/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivpropertiesdialog.h"

#include "asn1/types/builtintypes.h"
#include "asn1systemchecks.h"
#include "commands/cmdentityattributechange.h"
#include "commandsstack.h"
#include "contextparametersmodel.h"
#include "delegates/asn1valuedelegate.h"
#include "ifaceparametersmodel.h"
#include "interface/attributedelegate.h"
#include "itemeditor/common/ivutils.h"
#include "ivcomment.h"
#include "ivconnectiongroup.h"
#include "ivconnectiongroupmodel.h"
#include "ivinterface.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
#include "ivpropertieslistmodel.h"
#include "ivpropertiesview.h"
#include "ivpropertytemplateconfig.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"

#include <QDebug>
#include <QHeaderView>
#include <QListView>
#include <QPlainTextEdit>
#include <QTableView>
#include <QTimer>
#include <QUndoCommand>

namespace ive {

IVPropertiesDialog::IVPropertiesDialog(ivm::IVPropertyTemplateConfig *dynPropConfig, ivm::IVObject *obj,
        Asn1Acn::Asn1SystemChecks *asn1Checks, cmd::CommandsStack *commandsStack, QWidget *parent)
    : shared::PropertiesDialog(dynPropConfig, obj, asn1Checks, commandsStack, parent)
{
}

IVPropertiesDialog::~IVPropertiesDialog() { }

QString IVPropertiesDialog::objectTypeName() const
{
    const ivm::IVObject *obi = dataObject();
    if (!obi)
        return QString();

    switch (obi->type()) {
    case ivm::IVObject::Type::FunctionType:
        return tr("Function Type");
    case ivm::IVObject::Type::Function:
        return tr("Function");
    case ivm::IVObject::Type::RequiredInterface:
        return tr("RI");
    case ivm::IVObject::Type::ProvidedInterface:
        return tr("PI");
    case ivm::IVObject::Type::Comment:
        return tr("Comment");
    case ivm::IVObject::Type::Connection:
        return tr("Connection");
    case ivm::IVObject::Type::ConnectionGroup:
        return tr("Connection Group");
    default:
        return QString();
    }
}

void IVPropertiesDialog::init()
{
    if (!dataObject())
        return;

    shared::PropertiesDialog::init();

    switch (dataObject()->type()) {
    case ivm::IVObject::Type::FunctionType:
    case ivm::IVObject::Type::Function: {
        initContextParams();
        initAttributesView();
        break;
    }
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface: {
        initIfaceParams();
        initAttributesView();
        break;
    }
    case ivm::IVObject::Type::ConnectionGroup: {
        initConnectionGroup();
        initAttributesView();
        break;
    }
    case ivm::IVObject::Type::Comment: {
        initCommentView();
        break;
    }
    default:
        break;
    }
    setCurrentTabIndex(0);
}

ivm::IVObject *IVPropertiesDialog::dataObject() const
{
    return qobject_cast<ivm::IVObject *>(shared::PropertiesDialog::dataObject());
}

void IVPropertiesDialog::initConnectionGroup()
{
    auto model = new IVConnectionGroupModel(qobject_cast<ivm::IVConnectionGroup *>(dataObject()), commandMacro(), this);
    auto connectionsView = new QListView;
    connectionsView->setModel(model);
    insertTab(connectionsView, tr("Connections"));
}

void IVPropertiesDialog::initAttributesView()
{
    auto viewAttrs = new shared::AttributesView(this);
    shared::PropertiesListModel *modelAttrs { nullptr };
    QStyledItemDelegate *attrDelegate = new shared::AttributeDelegate(viewAttrs->tableView());

    switch (dataObject()->type()) {
    case ivm::IVObject::Type::FunctionType:
    case ivm::IVObject::Type::Function: {
        modelAttrs = new FunctionPropertiesListModel(commandMacro(), propertiesConfig(), this);
        break;
    }
    default:
        modelAttrs = new InterfacePropertiesListModel(commandMacro(), propertiesConfig(), this);
        break;
    }

    modelAttrs->setDataObject(dataObject());
    viewAttrs->tableView()->setItemDelegateForColumn(shared::PropertiesListModel::Column::Value, attrDelegate);
    viewAttrs->setModel(modelAttrs);

    insertTab(viewAttrs, tr("Attributes"));

    QTimer::singleShot(0, viewAttrs, [this, viewAttrs, modelAttrs]() {
        const int nameColumn = dataObject()->isFunction() ? FunctionPropertiesListModel::Column::Name
                                                          : InterfacePropertiesListModel::Column::Name;
        const int valueColumn = dataObject()->isFunction() ? FunctionPropertiesListModel::Column::Value
                                                           : InterfacePropertiesListModel::Column::Value;

        const QModelIndexList indexes = modelAttrs->match(modelAttrs->index(0, nameColumn),
                FunctionPropertiesListModel::DataRole, ivm::meta::Props::token(ivm::meta::Props::Token::name));
        if (!indexes.isEmpty()) {
            viewAttrs->tableView()->edit(indexes.front().siblingAtColumn(valueColumn));
        }
    });
}

void IVPropertiesDialog::initContextParams()
{
    ContextParametersModel *modelCtxParams = new ContextParametersModel(commandMacro(), this);
    modelCtxParams->setAsn1Check(asn1Checks());
    modelCtxParams->setDataObject(dataObject());

    shared::PropertiesViewBase *viewAttrs = new ContextParametersView(this);
    viewAttrs->tableView()->setItemDelegateForColumn(
            ContextParametersModel::Column::Type, new shared::AttributeDelegate(viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(
            ContextParametersModel::Column::Value, new Asn1ValueDelegate(asn1Checks(), viewAttrs->tableView()));
    viewAttrs->tableView()->horizontalHeader()->show();
    viewAttrs->setModel(modelCtxParams);
    insertTab(viewAttrs, tr("Context Parameters"));
}

void IVPropertiesDialog::initIfaceParams()
{
    IfaceParametersModel *modelIfaceParams =
            new IfaceParametersModel(commandMacro(), asn1Checks()->allTypeNames(), this);
    modelIfaceParams->setDataObject(dataObject());

    shared::PropertiesViewBase *viewAttrs = new IfaceParametersView(this);
    viewAttrs->tableView()->setItemDelegateForColumn(
            IfaceParametersModel::Column::Type, new shared::AttributeDelegate(viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(
            IfaceParametersModel::Column::Encoding, new shared::AttributeDelegate(viewAttrs->tableView()));
    viewAttrs->tableView()->setItemDelegateForColumn(
            IfaceParametersModel::Column::Direction, new shared::AttributeDelegate(viewAttrs->tableView()));
    viewAttrs->tableView()->horizontalHeader()->show();
    viewAttrs->setModel(modelIfaceParams);
    insertTab(viewAttrs, tr("Parameters"));
}

void IVPropertiesDialog::initCommentView()
{
    if (auto comment = qobject_cast<ivm::IVComment *>(dataObject())) {
        auto commentEdit = new QPlainTextEdit(this);
        commentEdit->setPlainText(comment->titleUI());
        insertTab(commentEdit, tr("Comment content"));
        connect(this, &QDialog::accepted, this, [comment, commentEdit, this]() {
            const QString text = commentEdit->toPlainText();
            if (comment->titleUI() == text)
                return;

            const QString encodedText = ivm::IVNameValidator::encodeName(comment->type(), text);
            const QVariantHash textArg { { ivm::meta::Props::token(ivm::meta::Props::Token::name), encodedText } };
            auto commentTextCmd = new shared::cmd::CmdEntityAttributeChange(comment, textArg);
            commentTextCmd->setText(tr("Edit Comment"));
            commandStack()->push(commentTextCmd);
        });
    }
}

} // namespace ive
