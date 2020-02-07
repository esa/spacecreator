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

#include "app/commandsstack.h"
#include "contextparametersmodel.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/functionattrdelegate.h"
#include "delegates/propertytypedelegate.h"
#include "ifaceparametersmodel.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectiface.h"
#include "ui_propertiesdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QTableView>

namespace taste3 {
namespace aadl {

PropertiesDialog::PropertiesDialog(AADLObject *obj, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setWindowTitle(tr("Edit Data"));

    initTabs();
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

QString PropertiesDialog::objectTypeName() const
{
    if (!m_dataObject)
        return QString();

    switch (m_dataObject->aadlType()) {
    case AADLObject::AADLObjectType::AADLFunctionType:
        return tr("Function Type");
    case AADLObject::AADLObjectType::AADLFunction:
        return tr("Function");
    case AADLObject::AADLObjectType::AADLIface: {
        QString ifaceDirection;
        if (auto iface = qobject_cast<AADLObjectIface *>(m_dataObject))
            ifaceDirection = iface->isProvided() ? tr("PI") : tr("RI");
        return ifaceDirection.isEmpty() ? tr("Interface") : ifaceDirection;
    }
    case AADLObject::AADLObjectType::AADLComment:
        return tr("Comment");
    case AADLObject::AADLObjectType::AADLConnection:
        return tr("Connection");
    default:
        return QString();
    }
}

void PropertiesDialog::open()
{
    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change properties"));
    QDialog::open();
}

void PropertiesDialog::done(int r)
{
    taste3::cmd::CommandsStack::current()->endMacro();

    if (QDialog::Rejected == r)
        taste3::cmd::CommandsStack::current()->undo();

    QDialog::done(r);
}

void PropertiesDialog::initTabs()
{
    if (!m_dataObject)
        return;

    auto initAttributesView = [this](const QString &title) {
        PropertiesListModel *modelAttrs = new PropertiesListModel(this);
        modelAttrs->setDataObject(m_dataObject);

        PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
        viewAttrs->setModel(modelAttrs);

        switch (m_dataObject->aadlType()) {
        case AADLObject::AADLObjectType::AADLFunction: {
            viewAttrs->tableView()->setItemDelegateForColumn(PropertiesListModel::ColumnValue,
                                                             new FunctionAttrDelegate(viewAttrs->tableView()));
            break;
        }
        default:
            break;
        }

        ui->tabWidget->insertTab(0, viewAttrs, tr("%1 Attributes").arg(title));
    };

    auto initContextParams = [this]() {
        ContextParametersModel *modelCtxParams = new ContextParametersModel(this);
        modelCtxParams->setDataObject(m_dataObject);

        PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
        viewAttrs->tableView()->setItemDelegateForColumn(ContextParametersModel::ColumnType,
                                                         new PropertyTypeDelegate(viewAttrs->tableView()));
        viewAttrs->tableView()->horizontalHeader()->show();
        viewAttrs->setModel(modelCtxParams);
        ui->tabWidget->insertTab(0, viewAttrs, tr("Context Parameters"));
    };

    auto initIfaceParams = [this]() {
        IfaceParametersModel *modelIfaceParams = new IfaceParametersModel(this);
        modelIfaceParams->setDataObject(m_dataObject);

        PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
        viewAttrs->tableView()->setItemDelegateForColumn(IfaceParametersModel::ColumnType,
                                                         new PropertyTypeDelegate(viewAttrs->tableView()));
        viewAttrs->tableView()->setItemDelegateForColumn(
                IfaceParametersModel::ColumnEncoding,
                new StringListComboDelegate({ tr("NATIVE"), tr("UPPER"), tr("ACN") }, // TODO: is it configurable?
                                            viewAttrs->tableView()));
        viewAttrs->tableView()->setItemDelegateForColumn(
                IfaceParametersModel::ColumnDirection,
                new StringListComboDelegate({ IfaceParameter::directionName(IfaceParameter::Direction::In),
                                              IfaceParameter::directionName(IfaceParameter::Direction::Out) },
                                            viewAttrs->tableView()));
        viewAttrs->tableView()->horizontalHeader()->show();
        viewAttrs->setModel(modelIfaceParams);
        ui->tabWidget->insertTab(0, viewAttrs, tr("Parameters"));
    };

    QString objectTypeLabel;

    switch (m_dataObject->aadlType()) {
    case AADLObject::AADLObjectType::AADLFunctionType: {
        objectTypeLabel = tr("Function Type");
        initContextParams();
        break;
    }
    case AADLObject::AADLObjectType::AADLFunction: {
        objectTypeLabel = tr("Function");
        initContextParams();
        break;
    }
    case AADLObject::AADLObjectType::AADLIface: {
        QString ifaceDirection;
        if (auto iface = qobject_cast<AADLObjectIface *>(m_dataObject))
            ifaceDirection = iface->isProvided() ? tr("PI") : tr("RI");
        objectTypeLabel = ifaceDirection.isEmpty() ? tr("Interface") : ifaceDirection;
        initIfaceParams();
        break;
    }
    case AADLObject::AADLObjectType::AADLComment: {
        objectTypeLabel = tr("Comment");
        break;
    }
    case AADLObject::AADLObjectType::AADLConnection: {
        objectTypeLabel = tr("Connection");
        break;
    }
    default:
        break;
    }

    initAttributesView(objectTypeLabel);
    ui->tabWidget->setCurrentIndex(0);
}

} // namespace aadl
} // namespace taste3
