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

#include "asn1sccoption.h"

#include "asn1sccoptionwidget.h"
#include "spacecreatorpluginconstants.h"

#include <QSettings>

namespace spctr {

Asn1SccOption::Asn1SccOption(QObject *parent)
    : Core::IOptionsPage(parent)
{
    setId(Constants::SETTINGS_ASN1_ID);
    setDisplayName(tr("Asn1scc compiler"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *Asn1SccOption::widget()
{
    if (!m_widget) {
        m_widget = new Asn1SccOptionWidget;

        QSettings settings;
        m_widget->setCompiler(settings.value(Constants::SETTINGS_ASN_COMPILER_KEY).toString());
        m_widget->setParameter(settings.value(Constants::SETTINGS_ASN_COMPILER_PARAMETER_KEY).toString());
    }
    return m_widget;
}

void Asn1SccOption::apply()
{
    if (!m_widget) {
        return;
    }

    QSettings setting;
    setting.setValue(Constants::SETTINGS_ASN_COMPILER_KEY, m_widget->compiler());
    setting.setValue(Constants::SETTINGS_ASN_COMPILER_PARAMETER_KEY, m_widget->parameter());
}

void Asn1SccOption::finish()
{
    delete m_widget;
}

} // namespace spctr
