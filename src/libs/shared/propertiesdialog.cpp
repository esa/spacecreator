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

#include "colors/colorhandlereditor.h"
#include "commands/cmdentityattributeschange.h"
#include "ui/veinteractiveobject.h"
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
    PropertiesDialogPrivate(
            ui::VEInteractiveObject *obj, PropertyTemplateConfig *dynPropConfig, cmd::CommandsStackBase *commandsStack)
        : uiObject(obj)
        , commandsStack(commandsStack)
        , dynPropConfig(dynPropConfig)
        , ui(new Ui::PropertiesDialog)
    {
    }

    QPointer<ui::VEInteractiveObject> uiObject;
    QPointer<cmd::CommandsStackBase> commandsStack;
    PropertyTemplateConfig *dynPropConfig;
    std::unique_ptr<Ui::PropertiesDialog> ui;
    std::unique_ptr<cmd::CommandsStackBase::Macro> cmdMacro;
};

PropertiesDialog::PropertiesDialog(PropertyTemplateConfig *dynPropConfig, ui::VEInteractiveObject *obj,
        cmd::CommandsStackBase *commandsStack, QWidget *parent)
    : QDialog(parent)
    , d(std::make_unique<PropertiesDialogPrivate>(obj, dynPropConfig, commandsStack))
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

void PropertiesDialog::initStyleView()
{
    if (auto obj = dataObject()) {
        auto colorHandlerEditor = new ColorHandlerEditor(this);
        colorHandlerEditor->setInteractiveObject(d->uiObject, propertiesConfig());
        insertTab(colorHandlerEditor, tr("Style configuration"), getTabCount());
        connect(colorHandlerEditor, &ColorHandlerEditor::entityAttributeChanged, this,
                [this](const EntityAttribute &entityAttr) {
                    auto styleChangeCmd =
                            new cmd::CmdEntityAttributesChange(propertiesConfig(), dataObject(), { entityAttr });
                    styleChangeCmd->setText(tr("Entity style configuration"));
                    commandStack()->push(styleChangeCmd);
                });
    }
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
    if (!dataObject())
        return;

    setWindowTitle(tr("Edit %1").arg(objectTypeName()));
    d->ui->tabWidget->setCurrentIndex(0);

    if (!d->cmdMacro) {
        d->cmdMacro = std::make_unique<cmd::CommandsStackBase::Macro>(
                d->commandsStack.data(), tr("Edit %1 - %2").arg(objectTypeName(), dataObject()->titleUI()));
    }
}

VEObject *PropertiesDialog::dataObject() const
{
    return d->uiObject->entity();
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

Ui::PropertiesDialog *PropertiesDialog::propertiesDialogUi() const
{
    return d->ui.get();
}

int PropertiesDialog::getTabCount() const
{
    return d->ui->tabWidget->count();
}

} // namespace ive
