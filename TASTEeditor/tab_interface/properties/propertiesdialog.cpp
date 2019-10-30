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
#include "propertieslistmodel.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectiface.h"
#include "ui_propertiesdialog.h"

#include <QDebug>
#include <QTableView>

namespace taste3 {
namespace aadl {

PropertiesDialog::PropertiesDialog(AADLObject *obj, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog)
    , m_dataObject(obj)
    , m_modelAttrs(new PropertiesListModel(this))
{
    ui->setupUi(this);

    m_modelAttrs->setDataObject(m_dataObject);

    ui->viewAttrs->setPropType(PropertiesListModel::ItemType::Attribute);
    ui->viewAttrs->setModel(m_modelAttrs);

    ui->viewProps->setPropType(PropertiesListModel::ItemType::Property);
    ui->viewProps->setModel(m_modelAttrs);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    const QString attrsTitle(tr("Attributes"));
    ui->tabWidget->setTabText(0, m_dataObject ? tr("%1 %2").arg(objectTypeName(), attrsTitle) : attrsTitle);
    ui->tabWidget->setTabText(1, tr("Context Parameters"));

    setWindowTitle(tr("Edit Data"));
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
            ifaceDirection = iface->isProvided() ? tr("Provided") : tr("Required");
        return ifaceDirection.isEmpty() ? tr("Interface") : tr("%1 Interface").arg(ifaceDirection);
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

} // namespace aadl
} // namespace taste3
