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

#include "adddynamicpropertydialog.h"
#include "baseitems/common/utils.h"
#include "common.h"
#include "dynamicproperty.h"
#include "ui_adddynamicpropertydialog.h"

#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

namespace aadlinterface {

AddDynamicPropertyDialog::AddDynamicPropertyDialog(const QStringList &prohibitedNames, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddDynamicPropertyDialog)
    , m_prohibitedNames(prohibitedNames)
{
    ui->setupUi(this);

    QSizePolicy keepWhenHidden = ui->teValues->sizePolicy();
    keepWhenHidden.setRetainSizeWhenHidden(true);
    ui->teValues->setSizePolicy(keepWhenHidden);

    m_nameColorDefault = ui->leName->palette().color(QPalette::Text);

    ui->comboTypes->addItem(tr("Integer"), QVariant::fromValue(DynamicProperty::Type::Integer));
    ui->comboTypes->addItem(tr("Real"), QVariant::fromValue(DynamicProperty::Type::Real));
    ui->comboTypes->addItem(tr("Boolean"), QVariant::fromValue(DynamicProperty::Type::Boolean));
    ui->comboTypes->addItem(tr("String"), QVariant::fromValue(DynamicProperty::Type::String));
    ui->comboTypes->addItem(tr("Enumeration"), QVariant::fromValue(DynamicProperty::Type::Enumeration));

    ui->teValues->setVisible(false);

    connect(ui->leName, &QLineEdit::textEdited, [this]() { validateName(false); });
    connect(ui->comboTypes, QOverload<int>::of(&QComboBox::activated), [this]() { validateType(); });
}

AddDynamicPropertyDialog::~AddDynamicPropertyDialog()
{
    delete ui;
}

bool AddDynamicPropertyDialog::validateName(const bool showWarn)
{
    QString warn;
    const QString &name = ui->leName->text().trimmed();
    const bool nameIsEmpty = name.isEmpty();
    if (nameIsEmpty)
        warn = tr("The name of new property can't be empty.");
    else if (m_prohibitedNames.contains(name))
        warn = tr("The name of new property must be unique.");

    const bool valid = warn.isEmpty();
    utils::setWidgetFontColor(ui->leName, valid ? m_nameColorDefault : Qt::red);

    if (!valid && showWarn)
        QMessageBox::warning(this, tr("Property name"), warn);

    return valid;
}

bool AddDynamicPropertyDialog::validateType()
{
    DynamicProperty::Type t = static_cast<DynamicProperty::Type>(ui->comboTypes->currentData().toInt());
    const bool isEnum(t == DynamicProperty::Type::Enumeration);
    if (isEnum) {
        ui->cbList->setChecked(false);
        ui->cbList->setEnabled(false);
    } else
        ui->cbList->setEnabled(true);

    ui->teValues->setVisible(isEnum);

    return true;
}

QStringList AddDynamicPropertyDialog::listValues() const
{
    const QString &values = ui->teValues->toPlainText().trimmed();
    return values.split(",", QString::SkipEmptyParts);
}

bool AddDynamicPropertyDialog::validateValuesList()
{
    QString warn;
    DynamicProperty::Type t = static_cast<DynamicProperty::Type>(ui->comboTypes->currentData().toInt());
    if (t == DynamicProperty::Type::Enumeration && listValues().isEmpty())
        warn = tr("Please specify at least one Enum value.");

    const bool ok(warn.isEmpty());

    if (!ok)
        QMessageBox::warning(this, tr("Enumeration"), warn);

    return ok;
}

bool AddDynamicPropertyDialog::validateScope()
{
    const bool ok(ui->cbFunction->isChecked() || ui->cbFunctionType->isChecked() || ui->cbInterface->isChecked());

    if (!ok)
        QMessageBox::warning(this, tr("Scope"), tr("Please select the property's scope"));

    return ok;
}

void AddDynamicPropertyDialog::accept()
{
    if (!validateName(true))
        return;

    if (!validateType())
        return;

    if (!validateValuesList())
        return;

    if (!validateScope())
        return;

    const QString &name = ui->leName->text().trimmed();
    const DynamicProperty::Type t = static_cast<DynamicProperty::Type>(ui->comboTypes->currentData().toInt());
    DynamicProperty::Scopes s;
    if (ui->cbFunction->isChecked())
        s |= DynamicProperty::Scope::Function;
    if (ui->cbFunctionType->isChecked())
        s |= DynamicProperty::Scope::FunctionType;
    if (ui->cbInterface->isChecked())
        s |= DynamicProperty::Scope::Interface;

    QVector<QVariant> list;
    if (t == DynamicProperty::Type::Enumeration || ui->cbList->isChecked())
        for (const QString &str : listValues())
            list.append(str.trimmed());

    m_attr = new DynamicProperty(name, t, s, list);
    QDialog::accept();
}

DynamicProperty *AddDynamicPropertyDialog::attribute() const
{
    return m_attr;
}

}
