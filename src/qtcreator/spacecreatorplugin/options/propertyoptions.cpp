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

#include "propertyoptions.h"

#include "interface/properties/propertytemplatewidget.h"
#include "propertytemplateconfig.h"
#include "spacecreatorpluginconstants.h"

namespace spctr {

PropertyOptions::PropertyOptions()
{
    setId(Constants::SETTINGS_IV_PROPERTIES_ID);
    setDisplayName(tr("Properties"));
    setCategory(Constants::SETTINGS_CATEGORY);

    setDisplayCategory(tr(Constants::SETTINGS_CATEGORY_DISPLAY));
}

QWidget *PropertyOptions::widget()
{
    if (!m_widget) {
        auto config = ivm::PropertyTemplateConfig::instance();
        config->init(ive::PropertyTemplateWidget::dynamicPropertiesFilePath());
        m_widget = new ive::PropertyTemplateWidget;
    }
    return m_widget;
}

void PropertyOptions::apply()
{
    m_widget->save();
    auto config = ivm::PropertyTemplateConfig::instance();
    config->init(ive::PropertyTemplateWidget::dynamicPropertiesFilePath());
}

void PropertyOptions::finish()
{
    delete m_widget;
}

}
