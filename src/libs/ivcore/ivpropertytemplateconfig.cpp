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

#include "ivpropertytemplateconfig.h"

#include "ivpropertytemplate.h"

namespace ivm {

const static QString kSysAttrsConfigFilePath = QLatin1String(":/defaults/resources/iv_system_attributes.xml");
const static QString kUserAttrsResourceConfigPath = QLatin1String(":/defaults/resources/iv_custom_attributes.xml");

IVPropertyTemplateConfig *IVPropertyTemplateConfig::m_instance = nullptr;

IVPropertyTemplateConfig *IVPropertyTemplateConfig::instance()
{
    if (m_instance == nullptr) {
        m_instance = new IVPropertyTemplateConfig;
    }
    return m_instance;
}

QString IVPropertyTemplateConfig::title() const
{
    return QObject::tr("InterfaceView");
}

shared::PropertyTemplate *IVPropertyTemplateConfig::createPropertyTemplate() const
{
    return new IVPropertyTemplate();
}

IVPropertyTemplateConfig::IVPropertyTemplateConfig()
    : shared::PropertyTemplateConfig(kSysAttrsConfigFilePath)
{
}

shared::PropertyTemplate *IVPropertyTemplateConfig::createPropertyTemplate(const QDomElement &element) const
{
    return shared::PropertyTemplate::createFromXml<IVPropertyTemplate>(element);
}

QString IVPropertyTemplateConfig::userAttrsResourceConfigPath() const
{
    return kUserAttrsResourceConfigPath;
}

} // namespace ivm
