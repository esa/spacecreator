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

#include "propertiesdialog.h"

#include "asn1systemchecks.h"
#include "ui_propertiesdialog.h"
#include "veobject.h"

#include <QDebug>
#include <QHeaderView>
#include <QListView>
#include <QPlainTextEdit>
#include <QPointer>
#include <QTableView>
#include <QTimer>
#include <QUndoCommand>

namespace shared {

struct PropertiesDialog::PropertiesDialogPrivate {
    PropertiesDialogPrivate(VEObject *obj, PropertyTemplateConfig *dynPropConfig, Asn1Acn::Asn1SystemChecks *asn1Checks,
            cmd::CommandsStackBase *commandsStack)
        : dataObject(obj)
        , asn1Checks(asn1Checks)
        , commandsStack(commandsStack)
        , dynPropConfig(dynPropConfig)
        , ui(new Ui::PropertiesDialog)
    {
    }

    QPointer<VEObject> dataObject;
    QPointer<Asn1Acn::Asn1SystemChecks> asn1Checks;
    QPointer<cmd::CommandsStackBase> commandsStack;
    PropertyTemplateConfig *dynPropConfig;
    std::unique_ptr<Ui::PropertiesDialog> ui;
    std::unique_ptr<cmd::CommandsStackBase::Macro> cmdMacro;
};

PropertiesDialog::PropertiesDialog(PropertyTemplateConfig *dynPropConfig, VEObject *obj,
        Asn1Acn::Asn1SystemChecks *asn1Checks, cmd::CommandsStackBase *commandsStack, QWidget *parent)
    : QDialog(parent)
    , d(std::make_unique<PropertiesDialogPrivate>(obj, dynPropConfig, asn1Checks, commandsStack))
{
    d->ui->setupUi(this);

    connect(d->ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(d->ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

PropertiesDialog::~PropertiesDialog() { }

void PropertiesDialog::insertTab(QWidget *widget, const QString &tabName, int idx)
{
    d->ui->tabWidget->insertTab(idx, widget, tabName);
}

void PropertiesDialog::setCurrentTabIndex(int idx)
{
    d->ui->tabWidget->setCurrentIndex(idx);
}

void PropertiesDialog::done(int r)
{
    if (!d->cmdMacro) {
        r = QDialog::Rejected;
    } else {
        d->cmdMacro->setComplete(r == QDialog::Accepted);
    }

    QDialog::done(r);
}

void PropertiesDialog::init()
{
    if (!d->dataObject)
        return;

    setWindowTitle(tr("Edit %1").arg(objectTypeName()));
    d->ui->tabWidget->setCurrentIndex(0);

    if (!d->cmdMacro) {
        d->cmdMacro = std::make_unique<cmd::CommandsStackBase::Macro>(
                d->commandsStack.data(), tr("Edit %1 - %2").arg(objectTypeName(), d->dataObject->titleUI()));
    }
}

VEObject *PropertiesDialog::dataObject() const
{
    return d->dataObject;
}

cmd::CommandsStackBase::Macro *PropertiesDialog::commandMacro() const
{
    return d->cmdMacro.get();
}

cmd::CommandsStackBase *PropertiesDialog::commandStack() const
{
    return d->commandsStack;
}

PropertyTemplateConfig *PropertiesDialog::propertiesConfig() const
{
    return d->dynPropConfig;
}

Asn1Acn::Asn1SystemChecks *PropertiesDialog::asn1Checks() const
{
    return d->asn1Checks;
}

} // namespace ive
