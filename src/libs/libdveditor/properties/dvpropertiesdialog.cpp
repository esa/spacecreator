/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvpropertiesdialog.h"

#include "abstractsystemchecks.h"
#include "asn1systemchecks.h"
#include "commandsstackbase.h"
#include "dvattributedelegate.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmessagebindingswidget.h"
#include "dvmodel.h"
#include "interface/attributedelegate.h"
#include "propertieslistmodel.h"
#include "propertiesviewbase.h"
#include "propertytemplateconfig.h"

#include <QStyledItemDelegate>
#include <QTableView>

namespace dve {

DVPropertiesDialog::DVPropertiesDialog(shared::PropertyTemplateConfig *dynPropConfig, dvm::DVObject *obj,
        dvm::AbstractSystemChecks *systemChecker, Asn1Acn::Asn1SystemChecks *asn1Checks,
        shared::cmd::CommandsStackBase *commandsStack, QWidget *parent)
    : shared::PropertiesDialog(dynPropConfig, obj, commandsStack, parent)
    , m_dvChecker(systemChecker)
    , m_asn1Checks(asn1Checks)
{
}

QString DVPropertiesDialog::objectTypeName() const
{
    if (dvm::DVObject *obj = dataObject()) {
        switch (obj->type()) {
        case dvm::DVObject::Type::Node:
            return tr("Node");
        case dvm::DVObject::Type::Partition:
            return tr("Partition");
        case dvm::DVObject::Type::Device:
            return tr("Device");
        case dvm::DVObject::Type::Connection:
            return tr("Connection");
        case dvm::DVObject::Type::Board:
            return tr("Board");
        case dvm::DVObject::Type::Function:
            return tr("Function");
        default:
            break;
        }
    }
    return {};
}

void DVPropertiesDialog::init()
{
    if (!dataObject())
        return;

    shared::PropertiesDialog::init();
    initAttributesView();

    switch (dataObject()->type()) {
    case dvm::DVObject::Type::Connection: {
        auto mBindings = new DVMessageBindingsWidget(commandMacro(), this);
        mBindings->initModel(qobject_cast<dvm::DVConnection *>(dataObject()), m_dvChecker);
        insertTab(mBindings, tr("Message Bindings"), -1);
        break;
    }
    case dvm::DVObject::Type::Device: {
        auto device = qobject_cast<dvm::DVDevice *>(dataObject());
        dvm::DVModel *model = device->model();
        Q_ASSERT(model);
        for (dvm::DVConnection *connection : model->connections(device)) {
            auto mBindings = new DVMessageBindingsWidget(commandMacro(), this);
            mBindings->initModel(connection, m_dvChecker);
            QString title = QString("%1 <-> %2")
                                    .arg(connection->sourceDevice() ? connection->sourceDevice()->titleUI() : "",
                                            connection->targetDevice() ? connection->targetDevice()->titleUI() : "");
            insertTab(mBindings, title, -1);
        }
        break;
    }
    default:
        break;
    }

    setCurrentTabIndex(0);
}

void DVPropertiesDialog::initAttributesView()
{
    shared::PropertiesListModel *modelAttrs = new shared::PropertiesListModel(commandMacro(), propertiesConfig(), this);
    modelAttrs->setDataObject(dataObject());

    shared::AttributesView *viewAttrs = new shared::AttributesView(this);
    QStyledItemDelegate *attrDelegate = new DVAttributeDelegate(m_asn1Checks, viewAttrs->tableView());

    viewAttrs->tableView()->setItemDelegateForColumn(shared::PropertiesListModel::Column::Value, attrDelegate);
    viewAttrs->setModel(modelAttrs);

    insertTab(viewAttrs, tr("Attributes"));
}

dvm::DVObject *DVPropertiesDialog::dataObject() const
{
    return qobject_cast<dvm::DVObject *>(shared::PropertiesDialog::dataObject());
}

} // namespace dve
