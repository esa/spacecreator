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

#include "dvpropertytemplateconfig.h"

#include "dvpropertytemplate.h"

namespace dvm {

const static QString kSysAttrsConfigFilePath = QLatin1String(":/defaults/resources/dv_system_attributes.xml");
const static QString kUserAttrsResourceConfigPath = QLatin1String(":/defaults/resources/dv_custom_attributes.xml");

DVPropertyTemplateConfig *DVPropertyTemplateConfig::m_instance = nullptr;

DVPropertyTemplateConfig *DVPropertyTemplateConfig::instance()
{
    if (m_instance == nullptr) {
        m_instance = new DVPropertyTemplateConfig;
    }
    return m_instance;
}

QString DVPropertyTemplateConfig::title() const
{
    return QObject::tr("DeploymentView");
}

shared::PropertyTemplate *DVPropertyTemplateConfig::createPropertyTemplate() const
{
    return new DVPropertyTemplate;
}

DVPropertyTemplateConfig::DVPropertyTemplateConfig()
    : shared::PropertyTemplateConfig(kSysAttrsConfigFilePath)
{
}

shared::PropertyTemplate *DVPropertyTemplateConfig::createPropertyTemplate(const QDomElement &element) const
{
    return shared::PropertyTemplate::createFromXml<DVPropertyTemplate>(element);
}

QString DVPropertyTemplateConfig::userAttrsResourceConfigPath() const
{
    return kUserAttrsResourceConfigPath;
}

} // namespace dvm
