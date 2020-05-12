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
#include "aadlobjectiface.h"
#include "commandsstack.h"
#include "contextparametersmodel.h"
#include "delegates/comboboxdelegate.h"
#include "delegates/functionattrdelegate.h"
#include "delegates/interfaceattrdelegate.h"
#include "delegates/propertytypedelegate.h"
#include "ifaceparametersmodel.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"
#include "ui_propertiesdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QTableView>

namespace aadlinterface {

PropertiesDialog::PropertiesDialog(aadl::AADLObject *obj, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
    , m_cmdMacro(new cmd::CommandsStack::Macro(
              tr("Edit %1 - %2")
                      .arg(aadl::AADLNameValidator::nameOfType(m_dataObject->aadlType()).trimmed(),
                              m_dataObject->title())))
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

    auto initAttributesView = [this]() {
        auto viewAttrs = new PropertiesViewBase(this);
        QStyledItemDelegate *modelDelegate { nullptr };
        PropertiesListModel *modelAttrs { nullptr };

        switch (m_dataObject->aadlType()) {
        case aadl::AADLObject::Type::Function: {
            modelAttrs = new FunctionPropertiesListModel(this);
            modelDelegate = new FunctionAttrDelegate(viewAttrs->tableView());
            break;
        }
        case aadl::AADLObject::Type::RequiredInterface:
        case aadl::AADLObject::Type::ProvidedInterface: {
            modelAttrs = new InterfacePropertiesListModel(this);
            modelDelegate = new InterfaceAttrDelegate(viewAttrs->tableView());
            break;
        }
        default:
            modelAttrs = new InterfacePropertiesListModel(this);
            break;
        }

        modelAttrs->setDataObject(m_dataObject);
        viewAttrs->setModel(modelAttrs);

        if (modelDelegate)
            viewAttrs->tableView()->setItemDelegateForColumn(PropertiesListModel::ColumnValue, modelDelegate);

        ui->tabWidget->insertTab(0, viewAttrs, tr("Attributes"));
    };

    auto initContextParams = [this]() {
        ContextParametersModel *modelCtxParams = new ContextParametersModel(this);
        modelCtxParams->setDataObject(m_dataObject);

        PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
        viewAttrs->tableView()->setItemDelegateForColumn(
                ContextParametersModel::ColumnType, new PropertyTypeDelegate(viewAttrs->tableView()));
        viewAttrs->tableView()->horizontalHeader()->show();
        viewAttrs->setModel(modelCtxParams);
        ui->tabWidget->insertTab(0, viewAttrs, tr("Context Parameters"));
    };

    auto initIfaceParams = [this]() {
        IfaceParametersModel *modelIfaceParams = new IfaceParametersModel(this);
        modelIfaceParams->setDataObject(m_dataObject);

        PropertiesViewBase *viewAttrs = new PropertiesViewBase(this);
        viewAttrs->tableView()->setItemDelegateForColumn(
                IfaceParametersModel::ColumnType, new PropertyTypeDelegate(viewAttrs->tableView()));
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
    };

    QString objectTypeLabel;

    switch (m_dataObject->aadlType()) {
    case aadl::AADLObject::Type::FunctionType: {
        objectTypeLabel = tr("Function Type");
        initContextParams();
        break;
    }
    case aadl::AADLObject::Type::Function: {
        objectTypeLabel = tr("Function");
        initContextParams();
        break;
    }
    case aadl::AADLObject::Type::RequiredInterface: {
        objectTypeLabel = tr("RI");
        initIfaceParams();
        break;
    }
    case aadl::AADLObject::Type::ProvidedInterface: {
        objectTypeLabel = tr("PI");
        initIfaceParams();
        break;
    }
    case aadl::AADLObject::Type::Comment: {
        objectTypeLabel = tr("Comment");
        break;
    }
    case aadl::AADLObject::Type::Connection: {
        objectTypeLabel = tr("Connection");
        break;
    }
    default:
        break;
    }

    setWindowTitle(tr("Edit %1").arg(objectTypeLabel));

    initAttributesView();
    ui->tabWidget->setCurrentIndex(0);
}

}
