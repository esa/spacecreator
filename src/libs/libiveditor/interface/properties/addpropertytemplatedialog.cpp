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

#include "addpropertytemplatedialog.h"

#include "baseitems/common/ivutils.h"
#include "common.h"
#include "propertytemplate.h"
#include "ui_addpropertytemplatedialog.h"

#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

namespace ive {

AddPropertyTemplateDialog::AddPropertyTemplateDialog(const QStringList &prohibitedNames, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddPropertyTemplateDialog)
    , m_prohibitedNames(prohibitedNames)
{
    ui->setupUi(this);

    QSizePolicy keepWhenHidden = ui->teValues->sizePolicy();
    keepWhenHidden.setRetainSizeWhenHidden(true);
    ui->teValues->setSizePolicy(keepWhenHidden);

    m_nameColorDefault = ui->leName->palette().color(QPalette::Text);

    ui->comboTypes->addItem(tr("Integer"), QVariant::fromValue(ivm::PropertyTemplate::Type::Integer));
    ui->comboTypes->addItem(tr("Real"), QVariant::fromValue(ivm::PropertyTemplate::Type::Real));
    ui->comboTypes->addItem(tr("Boolean"), QVariant::fromValue(ivm::PropertyTemplate::Type::Boolean));
    ui->comboTypes->addItem(tr("String"), QVariant::fromValue(ivm::PropertyTemplate::Type::String));
    ui->comboTypes->addItem(tr("Enumeration"), QVariant::fromValue(ivm::PropertyTemplate::Type::Enumeration));

    ui->teValues->setVisible(false);

    connect(ui->leName, &QLineEdit::textEdited, [this]() { validateName(false); });
    connect(ui->comboTypes, QOverload<int>::of(&QComboBox::activated), [this]() { validateType(); });
}

AddPropertyTemplateDialog::~AddPropertyTemplateDialog()
{
    delete ui;
}

bool AddPropertyTemplateDialog::validateName(const bool showWarn)
{
    QString warn;
    const QString &name = ui->leName->text().trimmed();
    const bool nameIsEmpty = name.isEmpty();
    if (nameIsEmpty)
        warn = tr("The name of new property can't be empty.");
    else if (m_prohibitedNames.contains(name))
        warn = tr("The name of new property must be unique.");

    const bool valid = warn.isEmpty();
    shared::setWidgetFontColor(ui->leName, valid ? m_nameColorDefault : Qt::red);

    if (!valid && showWarn)
        QMessageBox::warning(this, tr("Property name"), warn);

    return valid;
}

bool AddPropertyTemplateDialog::validateType()
{
    ivm::PropertyTemplate::Type t = static_cast<ivm::PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    const bool isEnum(t == ivm::PropertyTemplate::Type::Enumeration);
    ui->teValues->setVisible(isEnum);

    return true;
}

QStringList AddPropertyTemplateDialog::listValues() const
{
    const QString &values = ui->teValues->toPlainText().trimmed();
    return values.split(",", QString::SkipEmptyParts);
}

bool AddPropertyTemplateDialog::validateValuesList()
{
    QString warn;
    ivm::PropertyTemplate::Type t = static_cast<ivm::PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    if (t == ivm::PropertyTemplate::Type::Enumeration && listValues().isEmpty())
        warn = tr("Please specify at least one Enum value.");

    const bool ok(warn.isEmpty());

    if (!ok)
        QMessageBox::warning(this, tr("Enumeration"), warn);

    return ok;
}

bool AddPropertyTemplateDialog::validateScope()
{
    const bool ok(ui->cbFunction->isChecked() || ui->cbReqIface->isChecked() || ui->cbProvIface->isChecked());

    if (!ok)
        QMessageBox::warning(this, tr("Scope"), tr("Please select the property's scope"));

    return ok;
}

void AddPropertyTemplateDialog::accept()
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
    const ivm::PropertyTemplate::Type t =
            static_cast<ivm::PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    ivm::PropertyTemplate::Scopes s;
    if (ui->cbFunction->isChecked())
        s |= ivm::PropertyTemplate::Scope::Function;
    if (ui->cbReqIface->isChecked())
        s |= ivm::PropertyTemplate::Scope::Required_Interface;
    if (ui->cbProvIface->isChecked())
        s |= ivm::PropertyTemplate::Scope::Provided_Interface;
    if (ui->cbComment->isChecked())
        s |= ivm::PropertyTemplate::Scope::Comment;
    if (ui->cbConnection->isChecked())
        s |= ivm::PropertyTemplate::Scope::Connection;

    QList<QVariant> list;
    if (t == ivm::PropertyTemplate::Type::Enumeration)
        for (const QString &str : listValues())
            list.append(str.trimmed());

    const QString &pattern = ui->leValidationPattern->text().trimmed();

    const ivm::PropertyTemplate::Info i = ui->rbAttribute->isChecked() ? ivm::PropertyTemplate::Info::Attribute
                                                                        : ivm::PropertyTemplate::Info::Property;
    m_attr = new ivm::PropertyTemplate;
    m_attr->setName(name);
    m_attr->setInfo(i);
    m_attr->setType(t);
    m_attr->setScope(s);
    m_attr->setValue(list);
    m_attr->setValueValidatorPattern(pattern);
    QDialog::accept();
}

ivm::PropertyTemplate *AddPropertyTemplateDialog::attribute() const
{
    return m_attr;
}

} // namespace ive
