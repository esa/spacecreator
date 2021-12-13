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

#include "common.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "ui_addpropertytemplatedialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QHash>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include <cmath>

namespace shared {

AddPropertyTemplateDialog::AddPropertyTemplateDialog(
        PropertyTemplateConfig *config, const QStringList &prohibitedNames, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddPropertyTemplateDialog)
    , m_config(config)
    , m_prohibitedNames(prohibitedNames)
    , m_attr(config->createPropertyTemplate())
{
    ui->setupUi(this);

    QSizePolicy keepWhenHidden = ui->teValues->sizePolicy();
    keepWhenHidden.setRetainSizeWhenHidden(true);
    ui->teValues->setSizePolicy(keepWhenHidden);

    m_nameColorDefault = ui->leName->palette().color(QPalette::Text);

    ui->comboTypes->addItem(tr("Integer"), QVariant::fromValue(PropertyTemplate::Type::Integer));
    ui->comboTypes->addItem(tr("Real"), QVariant::fromValue(PropertyTemplate::Type::Real));
    ui->comboTypes->addItem(tr("Boolean"), QVariant::fromValue(PropertyTemplate::Type::Boolean));
    ui->comboTypes->addItem(tr("String"), QVariant::fromValue(PropertyTemplate::Type::String));
    ui->comboTypes->addItem(tr("Enumeration"), QVariant::fromValue(PropertyTemplate::Type::Enumeration));

    ui->teValues->setVisible(false);

    connect(ui->leName, &QLineEdit::textEdited, this, [this]() { validateName(false); });
    connect(ui->comboTypes, QOverload<int>::of(&QComboBox::activated), this, [this]() { validateType(); });

    const QMetaEnum me = m_attr->scopeMetaEnum();
    bool ok;

    const int columnCount = std::sqrt(me.keyCount());
    if (columnCount) {
        int index = 0;
        for (int keyIdx = 1; keyIdx < me.keyCount(); ++keyIdx, ++index) {
            const int intValue = me.keyToValue(me.key(keyIdx), &ok);
            if (!ok) {
                continue;
            }

            auto cbw = new QCheckBox(me.key(keyIdx), ui->gbScope);
            ui->gridLayout->addWidget(cbw, index / columnCount, index % columnCount);
            m_scopeMappings.append({ cbw, intValue });
        }
    }
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
    PropertyTemplate::Type t = static_cast<PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    const bool isEnum(t == PropertyTemplate::Type::Enumeration);
    ui->teValues->setVisible(isEnum);

    return true;
}

QStringList AddPropertyTemplateDialog::listValues() const
{
    const QString &values = ui->teValues->toPlainText().trimmed();
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    return values.split(QLatin1Char(','), QString::SkipEmptyParts);
#else
    return values.split(QLatin1Char(','), Qt::SkipEmptyParts);
#endif
}

bool AddPropertyTemplateDialog::validateValuesList()
{
    QString warn;
    const PropertyTemplate::Type t = static_cast<PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    if (t == PropertyTemplate::Type::Enumeration && listValues().isEmpty())
        warn = tr("Please specify at least one Enum value.");

    const bool ok(warn.isEmpty());

    if (!ok)
        QMessageBox::warning(this, tr("Enumeration"), warn);

    return ok;
}

bool AddPropertyTemplateDialog::validateScope()
{
    const bool ok = std::any_of(
            m_scopeMappings.cbegin(), m_scopeMappings.cend(), [](const QPair<QCheckBox *, int> &scopeMapping) {
                return scopeMapping.first && scopeMapping.first->isChecked();
            });

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

    m_attr->setName(ui->leName->text().trimmed());

    const PropertyTemplate::Info i =
            ui->rbAttribute->isChecked() ? PropertyTemplate::Info::Attribute : PropertyTemplate::Info::Property;
    m_attr->setInfo(i);

    const PropertyTemplate::Type t = static_cast<PropertyTemplate::Type>(ui->comboTypes->currentData().toInt());
    m_attr->setType(t);

    QList<QVariant> list;
    if (t == PropertyTemplate::Type::Enumeration) {
        for (const QString &str : listValues()) {
            list.append(str.trimmed());
        }
    }
    m_attr->setValue(list);

    int scopes = 0;
    std::for_each(
            m_scopeMappings.cbegin(), m_scopeMappings.cend(), [&scopes](const QPair<QCheckBox *, int> &scopeMapping) {
                if (scopeMapping.first && scopeMapping.first->isChecked()) {
                    scopes |= scopeMapping.second;
                }
            });
    m_attr->setScopes(scopes);

    m_attr->setValueValidatorPattern(ui->leValidationPattern->text().trimmed());

    QDialog::accept();
}

PropertyTemplate *AddPropertyTemplateDialog::attribute() const
{
    return m_attr.get();
}

} // namespace shared
